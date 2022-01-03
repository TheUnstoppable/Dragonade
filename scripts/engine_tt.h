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
#pragma once

#include "scripts.h"
#include "engine_ttdef.h"
#include "Vector2.h"
#include "Vector4.h"
#include "GameDefinition.h"
#include "HashTemplateClass.h"
#include "SList.h"
#include "cPlayer.h"
#include "PurchaseSettingsDefClass.h"
class ConnectionAcceptanceFilter;
class WideStringClass;
class cScTextObj;
class SCAnnouncement;
class cPlayerKill;
class cPurchaseResponseEvent;
//start per-player script commands stuff
typedef void (*_Enable_Stealth_Player) (GameObject *obj,GameObject *obj2,bool enable);
typedef void (*_Set_Fog_Enable_Player) (GameObject *obj,bool enable);
typedef void (*_Set_Fog_Range_Player) (GameObject *obj,float Start_Distance,float End_Distance,float transition);
typedef void (*_Set_Background_Music_Player) (GameObject *obj,const char *music);
typedef void (*_Fade_Background_Music_Player) (GameObject *obj,const char *music,int unk1,int unk2);
typedef void (*_Stop_Background_Music_Player) (GameObject *obj);
typedef void (*_Enable_Radar_Player) (GameObject *obj,bool enable);
typedef void (*_Display_GDI_Player_Terminal_Player) (GameObject *obj);
typedef void (*_Display_NOD_Player_Terminal_Player) (GameObject *obj);
typedef void (*_Set_Screen_Fade_Color_Player) (GameObject *obj,float red,float green,float blue,float transition);
typedef void (*_Set_Screen_Fade_Opacity_Player) (GameObject *obj,float opacity,float transition);
typedef void (*_Force_Camera_Look_Player) (GameObject *obj,const Vector3 & look);
typedef void (*_Enable_HUD_Player) (GameObject *obj,bool enable);
typedef int (*_Create_Sound_Player) (GameObject *obj,const char *soundname,const Vector3 & position,GameObject *obj2);
typedef int (*_Create_2D_Sound_Player) (GameObject *obj,const char *soundname);
typedef int (*_Create_2D_WAV_Sound_Player) (GameObject *obj,const char *soundname);
typedef int (*_Create_3D_WAV_Sound_At_Bone_Player) (GameObject *obj,const char *soundname,GameObject *obj2,const char *bonename);
typedef int (*_Create_3D_Sound_At_Bone_Player) (GameObject *obj,const char *soundname,GameObject *obj2,const char *bonename);
typedef int (*aoch) (ObjectCreateHookStruct *h);
typedef void (*roch) (int pos);
typedef int (*akh) (KeyHookStruct *h);
typedef void (*rkh) (int pos);
typedef void (*ss) (GameObject *obj,int scope);
typedef void (*ss2) (GameObject *obj,ScriptClass *script,int number);
typedef bool (*htm) (GameObject *obj,ScriptClass *script,int number);
typedef void (*ach) (ChatHook h);
typedef void (*ahh) (HostHook h);
typedef void (*arh) (RefillHook h);
typedef void (*allh) (LoadLevelHook h);
typedef int (*aph) (PurchaseHook h,const char *data);
typedef int (*apmh) (PurchaseMonHook h,const char *data);
typedef void (*rph) (int pos);
typedef void (*apjh) (PlayerJoin h);
typedef void (*aplh) (PlayerLeave h);
typedef void (*arch) (RadioHook h);
typedef void (*asdh) (StockDamageHook h);
typedef void (*atdh) (TtDamageHook h);
typedef const char *(*gcmt) ();
typedef void (*sit) (GameObject *obj,const char *texture);
typedef void (*cit) (GameObject *obj);
typedef void (*svl) (unsigned int limit);
typedef unsigned long (*gvl) ();
typedef void (*sm) (unsigned int red,unsigned int green,unsigned int blue, const char *msg);
typedef void (*smp) (GameObject *obj,unsigned int red,unsigned int green,unsigned int blue, const char *msg);
typedef void (*sw) (int mode);
typedef void (*lnhi) (GameObject *obj,const char *ini);
typedef void (*rw) (GameObject *obj,const char *weapon);
typedef void (*crm) (float scale,float offsetx,float offsety,const char *texture);
typedef float (*gbm) (int team);
typedef void (*scb) (bool building,int team);
typedef bool (*icb) (int team);
typedef void (*acoh) (ConsoleOutputHook h);
typedef void (*sfc) (unsigned int red, unsigned int green, unsigned int blue);
typedef void (*sfcp) (GameObject *obj,unsigned int red, unsigned int green, unsigned int blue);
typedef void (*sfm) (unsigned int mode);
typedef void (*sfmp) (GameObject *obj,unsigned int mode);
typedef void (*sfd) (float density);
typedef void (*sfdp) (GameObject *obj,float density);
typedef void (*ctr) (float time);
typedef void (*ctl) (int time);
typedef void (*dps) (GameObject *obj);
typedef void (*shn) (GameObject *obj,unsigned int number);
typedef GameObject *(*geo) ();
typedef void (*seo) (GameObject *obj);
typedef int (*asn) (ShaderNotifyStruct *s);
typedef int (*rsn) (int pos);
typedef void (*dod) (GameObject *obj,const char *file);
typedef void (*sl) (unsigned int limit);
typedef unsigned int (*gl) ();
typedef void (*_Get_Fog_Range) (float &Start_Distance,float &End_Distance);
typedef void (*ssp) (const char* parameter,const char *value);
typedef void (*sspp) (GameObject* player,const char* parameter,const char* value);
typedef void (*sspo) (const char* parameter,const char* value,GameObject* obj);
typedef void (*sspop) (GameObject* player,const char* parameter,const char* value,GameObject* obj);
typedef void (*ssn) (const char *name);
typedef const char *(*gsnx) ();
typedef void (*addConnectionAcceptanceFilterType)(ConnectionAcceptanceFilter* connectionAcceptanceFilters);
typedef bool (*removeConnectionAcceptanceFilterType)(ConnectionAcceptanceFilter* connectionAcceptanceFilters);
typedef const char *(*Get_IP_Addressx) (int PlayerID);
typedef const char *(*Get_IP_Portx) (int PlayerID);
typedef const sockaddr_in *(*Get_IPx) (int PlayerID);
typedef int (*Get_Bandwidthx) (int PlayerID);
typedef int (*Get_Pingx) (int PlayerID);
typedef unsigned long (*Get_Kbitsx) (int PlayerID);
typedef void (*sfop) (GameObject *obj,float start, float end);
typedef void (*cfop) (GameObject *obj);
typedef void (*dgh) (GameObject *obj,int EventID,const char *Sound,int TitleID,int TextID,int TextID2,int TextID3);
typedef void (*dghi) (GameObject *obj,int EventID,const char *Sound,int TitleID,int TextID,int TextID2,int TextID3,const char *TextureName);
typedef void (*sclb) (GameObject *obj,int bullets);
typedef void (*spb) (GameObject *obj,int position,int bullets);
typedef void (*sb) (GameObject *obj,const char *weapon,int bullets);
typedef void (*smie) (bool earth);
typedef int (*gml) ();
typedef void (*sbd) ();
typedef float (*gpv) (int playerId);
typedef uint (*gpr) (int playerId);
typedef unsigned int (*gdw) ();
typedef bool (*gdie) ();
typedef void (*naee) (GameObject * obj, const ActionParamsStruct & params);
typedef GameObject *(*ggobpn) (const char *name);
typedef void (*scap) (int type,GameObject *sender,int team);
typedef float (*stc) (float percentage, int team);
typedef float (*gtc) (int team);
typedef void (*ec) (int clientId, const WideStringClass& reason);
typedef const char * (*gcsh) (int clientId);
typedef int (*gcmi) ();
typedef const char * (*gm) (int index);
typedef const int (*gmc) ();
typedef bool (*smx) (const char *map,int index);
typedef void (*ggd) (HashTemplateClass<StringClass, GameDefinition> &definitions);
typedef SList<cPlayer> *(*gpl) ();
typedef void (*uno) (NetworkObjectClass *obj);
typedef void (*unop) (NetworkObjectClass *Object,int ID);
typedef cScTextObj *(*sct) (const WideStringClass& message, TextMessageEnum type, bool popup, int senderId, int receiverId, bool dodirtybit, bool doact);
typedef SCAnnouncement *(*sca) (int _receiptientId, int _senderId, int _translationId, AnnouncementEnum _type, int _emoticonId, bool dodirtybit, bool doact);
typedef cPlayerKill *(*spkm) (int Killer, int Victim);
typedef cPurchaseResponseEvent *(*spr) (int ID, int Type);
typedef bool (*iwu) (wchar_t *WideName);
typedef void (*cee) (const char *Explosion, Vector3 &Pos, GameObject *Creator);
typedef void (*rwpa) (DynamicVectorClass<int> &WaypathIDs);
typedef void (*rwpo) (int WaypathID, DynamicVectorClass<int> &WaypointIDs);
typedef void (*gwp) (int WaypathID, int WaypointID, Vector3 &Pos);
typedef bool (*trc) (const Vector3 &pos1, const Vector3 &pos2, const bool checkDynamicObjects, Vector3 *contactPoint, GameObject *compareObj);
typedef bool (*iib) (const AABoxClass &box, const Vector3 &pos);
typedef bool (*cgst) (Vector3 position);
typedef void (*cl) (const AmmoDefinitionClass *ammodef,Vector3 &start,Vector3 &end);
typedef void (*gc) (float &cloudcover, float &gloominess);
typedef void (*gli) (float &intensity, float &startdistance, float &enddistance, float &heading, float &distribution);
typedef void (*gw) (float &heading, float &speed, float &variability);
typedef void (*gp) (float &density);
typedef void (*ugo) (int ID);
typedef double (*gttv) ();
typedef uint (*gttr) ();
typedef void (*htp) (GameObject *obj, int string, Vector3 color);
typedef void (*csp) (GameObject *obj,int id,const char *string);
typedef void (*fpu) (GameObject *obj);
typedef bool (*grps) (const Vector3 &center, float range, Vector3 *returnPosition);
typedef void (*htpt) (GameObject *obj, int replaceStringId,const char *replaceText, Vector3 color);
typedef void (*grw) (GameObject *object,const char *GrantWeaponName,bool GrantWeapon, int GrantWeaponRounds, bool GrantWeaponClips);
typedef void (*sovfp) (GameObject *player,int objectId,bool visible);
typedef void (*sov) (int objectId,bool visible);
typedef void (*lscg) (GameObject *obj,Collision_Group_Type collisionGroupType);
typedef void (*ulscg) (GameObject *obj);
typedef int (*cwsdp) (GameObject *obj,const char *soundname);
typedef void (*fpup) (GameObject *player,GameObject *obj);
typedef bool (*iea) (GameObject *obj);
typedef void (*sbgmo) (GameObject *obj,const char *music,float startoffset);
typedef void (*setcam) (GameObject *obj,const char *camera);
typedef void (*settran) (GameObject *obj,int defid,int stringid);
typedef void (*setnur) (GameObject *obj,int rate);
typedef void (*fou) (GameObject *obj);
typedef void (*foup) (GameObject *player,GameObject *obj);
typedef int (*cwscp) (GameObject *obj,const char *soundname);
typedef void (*ftu) (GameObject *obj);
typedef void (*ftup) (GameObject *player,GameObject *obj);
typedef void (*fvu) (GameObject *obj);
typedef void (*fvup) (GameObject *player,GameObject *obj);
typedef void (*schn) (GameObject *obj);
typedef uint32(*gpd) (const Vector3& start, const Vector3& dest, PathfindDistanceCallback callback, SmartGameObj* pathObj, void* data);
typedef uint32 (*gpda) (SmartGameObj* pathObj, const Vector3 &dest, PathfindDistanceCallback callback, void *data);
typedef bool(*gpdb) (SmartGameObj* pathObj, const Vector3& dest, float& distanceResult, PathfindDistanceResult& pathfindResult);
typedef bool (*cgpd) (uint32 id);
typedef void (*gmsl) (sint32 team, SoldierGameObj* soldier, Matrix3D &location);
typedef void (*esbn) (const char *name, bool enable);
typedef bool (*ipg) ();
typedef bool (*gcps) (const Vector3 &center, float maxDistance, Vector3 *returnPosition);
typedef bool (*gcpss) (const Vector3 &center, float maxDistance, Vector3 *returnPosition, float minSectorSize);
typedef int (*grcs) (int index);
typedef int (*sei) (SoldierGameObj *obj, int emoticonId);
typedef bool (*kmd) ();
typedef bool (*ise) ();
typedef bool (*iee) ();
typedef bool (*cbg) (int team);
typedef bool (*cba) (int team);
typedef bool (*cbn) (int team);
typedef bool (*isb) (SoldierGameObj *obj);
typedef bool (*ioep) (BeaconGameObj *obj);
typedef void (*fcpp) (BuildingGameObj *building, Vector3 &pos, Vector3 *polyPos);
typedef void (*sdd) (int text_id, SoldierGameObj* speaker, int* sound_id);
typedef void (*sddp) (GameObject* player, int text_id, SoldierGameObj* speaker, int* sound_id);
typedef void (*sddt) (int text_id, SoldierGameObj* speaker, int* sound_id, int team);
typedef void (*elp) (GameObject* player, bool onoff, float seconds);
typedef int (*cst) (const char* soundname, const Vector3& position, GameObject* obj2, int team);
typedef int (*c2dst) (const char* soundname, int team);
typedef int (*c2dwst) (const char* soundname, int team);
typedef int (*c2dwstd) (const char* soundname, int team);
typedef int (*c2dwstc) (const char* soundname, int team);
typedef int (*c3dwsbt) (const char* soundname, GameObject* obj, const char* bonename, int team);
typedef int (*c3dsbt) (const char* soundname, GameObject* obj, const char* bonename, int team);
typedef int (*_Stop_Sound_Player) (GameObject* player, int sound_id, bool destroy_sound);
typedef int (*_Stop_Sound_Team) (int sound_id, bool destroy_sound, int team);
typedef int (*ssa) (GameObject *obj,const char *animation,bool looping,const char *subobject,float startFrame,float endFrame,bool blended);
typedef void (*sts) (float scale);
typedef int (*ssap) (GameObject *player,GameObject *obj,const char *animation,bool looping,const char *subobject,float startFrame,float endFrame,bool blended);
typedef int (*wfa) (const char *_FileName, const char *_Text);
SCRIPTS_API extern gpl Get_Player_List;
SCRIPTS_API extern gcmi Get_Current_Map_Index;
SCRIPTS_API extern gm Get_Map;
SCRIPTS_API extern gmc Get_Map_Count;
SCRIPTS_API extern smx Set_Map;
SCRIPTS_API extern ggd Get_Game_Definitions;
SCRIPTS_API extern ec Evict_Client;
SCRIPTS_API extern gcsh Get_Client_Serial_Hash;
SCRIPTS_API extern ggobpn Get_GameObj_By_Player_Name;
SCRIPTS_API extern scap Send_Custom_All_Players;
SCRIPTS_API extern stc Steal_Team_Credits;
SCRIPTS_API extern gtc Get_Team_Credits;
SCRIPTS_API extern sclb Set_Current_Bullets;
SCRIPTS_API extern sclb Set_Current_Clip_Bullets;
SCRIPTS_API extern spb Set_Position_Bullets;
SCRIPTS_API extern spb Set_Position_Clip_Bullets;
SCRIPTS_API extern sb Set_Bullets;
SCRIPTS_API extern sb Set_Clip_Bullets;
SCRIPTS_API extern dgh Display_Game_Hint;
SCRIPTS_API extern dghi Display_Game_Hint_Image;
SCRIPTS_API extern asn AddShaderNotify;
SCRIPTS_API extern rsn RemoveShaderNotify;
SCRIPTS_API extern ach AddChatHook;
SCRIPTS_API extern ahh AddHostHook;
SCRIPTS_API extern arh AddRefillHook;
SCRIPTS_API extern ss Set_Scope;
SCRIPTS_API extern aoch AddObjectCreateHook;
SCRIPTS_API extern roch RemoveObjectCreateHook;
SCRIPTS_API extern akh AddKeyHook;
SCRIPTS_API extern rkh RemoveKeyHook;
SCRIPTS_API extern gcmt GetCurrentMusicTrack;
SCRIPTS_API extern allh AddLoadLevelHook;
SCRIPTS_API extern allh AddPreLoadLevelHook;
SCRIPTS_API extern allh AddGameOverHook;
SCRIPTS_API extern allh AddThinkHook;
SCRIPTS_API extern apjh AddPlayerJoinHook;
SCRIPTS_API extern arch AddRadioHook;
SCRIPTS_API extern asdh AddStockDamageHook;
SCRIPTS_API extern atdh AddTtDamageHook;
SCRIPTS_API extern aplh AddPlayerLeaveHook;
SCRIPTS_API extern aph AddPowerupPurchaseHook;
SCRIPTS_API extern aph AddVehiclePurchaseHook;
SCRIPTS_API extern aph AddCharacterPurchaseHook;
SCRIPTS_API extern apmh AddPowerupPurchaseMonHook;
SCRIPTS_API extern apmh AddVehiclePurchaseMonHook;
SCRIPTS_API extern apmh AddCharacterPurchaseMonHook;
SCRIPTS_API extern rph RemovePowerupPurchaseHook;
SCRIPTS_API extern rph RemoveVehiclePurchaseHook;
SCRIPTS_API extern rph RemoveCharacterPurchaseHook;
SCRIPTS_API extern rph RemovePowerupPurchaseMonHook;
SCRIPTS_API extern rph RemoveVehiclePurchaseMonHook;
SCRIPTS_API extern rph RemoveCharacterPurchaseMonHook;
SCRIPTS_API extern sit Set_Info_Texture;
SCRIPTS_API extern cit Clear_Info_Texture;
SCRIPTS_API extern _Enable_Stealth_Player Enable_Stealth_Player;
SCRIPTS_API extern _Set_Fog_Enable_Player Set_Fog_Enable_Player;
SCRIPTS_API extern _Set_Fog_Range_Player Set_Fog_Range_Player;
SCRIPTS_API extern _Set_Background_Music_Player Set_Background_Music_Player;
SCRIPTS_API extern _Fade_Background_Music_Player Fade_Background_Music_Player;
SCRIPTS_API extern _Stop_Background_Music_Player Stop_Background_Music_Player;
SCRIPTS_API extern _Enable_Radar_Player Enable_Radar_Player;
SCRIPTS_API extern _Display_GDI_Player_Terminal_Player Display_GDI_Player_Terminal_Player;
SCRIPTS_API extern _Display_NOD_Player_Terminal_Player Display_NOD_Player_Terminal_Player;
SCRIPTS_API extern _Set_Screen_Fade_Color_Player Set_Screen_Fade_Color_Player;
SCRIPTS_API extern _Set_Screen_Fade_Opacity_Player Set_Screen_Fade_Opacity_Player;
SCRIPTS_API extern _Force_Camera_Look_Player Force_Camera_Look_Player;
SCRIPTS_API extern _Enable_HUD_Player Enable_HUD_Player;
SCRIPTS_API extern _Create_Sound_Player Create_Sound_Player;
SCRIPTS_API extern _Create_2D_Sound_Player Create_2D_Sound_Player;
SCRIPTS_API extern _Create_2D_WAV_Sound_Player Create_2D_WAV_Sound_Player;
SCRIPTS_API extern _Create_3D_WAV_Sound_At_Bone_Player Create_3D_WAV_Sound_At_Bone_Player;
SCRIPTS_API extern _Create_3D_Sound_At_Bone_Player Create_3D_Sound_At_Bone_Player;
SCRIPTS_API extern svl Set_Vehicle_Limit;
SCRIPTS_API extern gvl Get_Vehicle_Limit;
SCRIPTS_API extern svl Set_Air_Vehicle_Limit;
SCRIPTS_API extern gvl Get_Air_Vehicle_Limit;
SCRIPTS_API extern svl Set_Naval_Vehicle_Limit;
SCRIPTS_API extern gvl Get_Naval_Vehicle_Limit;
SCRIPTS_API extern sm Send_Message;
SCRIPTS_API extern smp Send_Message_Player;
SCRIPTS_API extern lnhi Load_New_HUD_INI;
SCRIPTS_API extern rw Remove_Weapon;
SCRIPTS_API extern crm Change_Radar_Map;
SCRIPTS_API extern gbm Get_Build_Time_Multiplier;
SCRIPTS_API extern scb Set_Currently_Building;
SCRIPTS_API extern icb Is_Currently_Building;
SCRIPTS_API extern acoh AddConsoleOutputHook;
SCRIPTS_API extern acoh AddLogFileHook;
SCRIPTS_API extern sfc Set_Fog_Color;
SCRIPTS_API extern sfcp Set_Fog_Color_Player;
SCRIPTS_API extern sfm Set_Fog_Mode;
SCRIPTS_API extern sfmp Set_Fog_Mode_Player;
SCRIPTS_API extern sfd Set_Fog_Density;
SCRIPTS_API extern sfdp Set_Fog_Density_Player;
SCRIPTS_API extern ctr Change_Time_Remaining;
SCRIPTS_API extern ctl Change_Time_Limit;
SCRIPTS_API extern dps Display_Security_Dialog;
SCRIPTS_API extern shn Send_HUD_Number;
SCRIPTS_API extern geo GetExplosionObj;
SCRIPTS_API extern seo SetExplosionObj;
SCRIPTS_API extern uno Update_Network_Object;
SCRIPTS_API extern unop Update_Network_Object_Player;
SCRIPTS_API extern sct Send_Client_Text;
SCRIPTS_API extern sca Send_Client_Announcement;
SCRIPTS_API extern spkm Send_Player_Kill_Message;
SCRIPTS_API extern spr Send_Purchase_Response;
SCRIPTS_API extern dod Do_Objectives_Dlg;
SCRIPTS_API extern sl Set_Player_Limit;
SCRIPTS_API extern gl Get_Player_Limit;
SCRIPTS_API extern _Get_Fog_Range Get_Fog_Range;
SCRIPTS_API extern sspp Send_Shader_Param_Player; //dont use, use the other calls below instead
SCRIPTS_API extern ssp Send_Shader_Param; //dont use, use the other calls below instead
SCRIPTS_API extern sspop Send_Shader_Param_Obj_Player; //dont use, use the other calls below instead
SCRIPTS_API extern sspo Send_Shader_Param_Obj; //dont use, use the other calls below instead
SCRIPTS_API extern ssn Set_GDI_Soldier_Name;
SCRIPTS_API extern ssn Set_Nod_Soldier_Name;
SCRIPTS_API extern gsnx Get_GDI_Soldier_Name;
SCRIPTS_API extern gsnx Get_Nod_Soldier_Name;
SCRIPTS_API extern Get_IP_Addressx Get_IP_Address;
SCRIPTS_API extern Get_IP_Portx Get_IP_Port;
SCRIPTS_API extern Get_IPx Get_IP;
SCRIPTS_API extern Get_Bandwidthx Get_Bandwidth;
SCRIPTS_API extern Get_Pingx Get_Ping;
SCRIPTS_API extern Get_Kbitsx Get_Kbits;
SCRIPTS_API extern sfop Set_Fog_Override_Player;
SCRIPTS_API extern cfop Clear_Fog_Override_Player;
SCRIPTS_API extern smie Set_Moon_Is_Earth;
SCRIPTS_API extern smie Set_Global_Stealth_Disable;
SCRIPTS_API extern gml Get_Mine_Limit;
SCRIPTS_API extern sbd Set_Special_Base_Destruction;
SCRIPTS_API extern gpv Get_Client_Version;
SCRIPTS_API extern gpr Get_Client_Revision;
SCRIPTS_API extern gpr Get_Revision;
SCRIPTS_API extern gdw Get_Damage_Warhead;
SCRIPTS_API extern gdie Get_Damage_Is_Explosion;
SCRIPTS_API extern addConnectionAcceptanceFilterType addConnectionAcceptanceFilter;
SCRIPTS_API extern removeConnectionAcceptanceFilterType removeConnectionAcceptanceFilter;
SCRIPTS_API extern iwu Is_WOL_User;
SCRIPTS_API extern gml Get_Tech_Level;
SCRIPTS_API extern sw Set_Tech_Level;
SCRIPTS_API extern cee Create_Explosion_Extended;
SCRIPTS_API extern rwpa Retrieve_Waypaths;
SCRIPTS_API extern rwpo Retrieve_Waypoints;
SCRIPTS_API extern gwp Get_Waypoint_Position;
SCRIPTS_API extern trc Test_Raycast_Collision;
SCRIPTS_API extern iib Is_Inside_AABox;
SCRIPTS_API extern cgst Can_Generic_Soldier_Teleport;
SCRIPTS_API extern cl Create_Lightning;
SCRIPTS_API extern gc Get_Clouds;
SCRIPTS_API extern gli Get_Lightning;
SCRIPTS_API extern gw Get_Wind;
SCRIPTS_API extern gp Get_Rain;
SCRIPTS_API extern gp Get_Snow;
SCRIPTS_API extern gp Get_Ash;
SCRIPTS_API extern ugo Update_Game_Options;
SCRIPTS_API extern gttv GetTTVersion;
SCRIPTS_API extern gttr GetTTRevision;
SCRIPTS_API extern htp Set_HUD_Help_Text_Player;
SCRIPTS_API extern csp Change_String_Player;
SCRIPTS_API extern fpu Force_Position_Update;
SCRIPTS_API extern grps Get_Random_Pathfind_Spot;
SCRIPTS_API extern htpt Set_HUD_Help_Text_Player_Text;
SCRIPTS_API extern grw Grant_Weapon;
SCRIPTS_API extern sovfp Set_Object_Visibility_For_Player;
SCRIPTS_API extern sov Set_Object_Visibility;
SCRIPTS_API extern lscg Lock_Soldier_Collision_Group;
SCRIPTS_API extern ulscg Unlock_Soldier_Collision_Group;
SCRIPTS_API extern iea Is_Engine_Enabled;
SCRIPTS_API extern ss Stop_Timer;
SCRIPTS_API extern cwsdp Create_2D_Wave_Sound_Dialog_Player;
SCRIPTS_API extern fpup Force_Position_Update_Player;
SCRIPTS_API extern ss2 Stop_Timer2;
SCRIPTS_API extern htm Has_Timer;
SCRIPTS_API extern sbgmo Set_Background_Music_Player_Offset;
SCRIPTS_API extern setcam Set_Camera_Player;
SCRIPTS_API extern settran Set_Definition_TranslationID_Player;
SCRIPTS_API extern setnur Set_Net_Update_Rate_Player;
SCRIPTS_API extern fou Force_Orientation_Update;
SCRIPTS_API extern foup Force_Orientation_Update_Player;
SCRIPTS_API extern cwscp Create_2D_Wave_Sound_Cinematic_Player;
SCRIPTS_API extern ftu Force_Turret_Update;
SCRIPTS_API extern ftup Force_Turret_Update_Player;
SCRIPTS_API extern fvu Force_Velocity_Update;
SCRIPTS_API extern fvup Force_Velocity_Update_Player;
SCRIPTS_API extern schn Set_Camera_Host_Network;
SCRIPTS_API extern gpd Get_Pathfind_Distance; // Deprecated, use Get_Pathfind_Distance_Async in stead
SCRIPTS_API extern gpda Get_Pathfind_Distance_Async; // Returns > 0 when succusful. pathObj must be a vehicle or a soldier
SCRIPTS_API extern cgpd Cancel_Get_Pathfind_Distance; // Cancels a pathfind distance request. Use the result from Get_Pathfind_Distance_Async/Get_Pathfind_Distance to cancel
SCRIPTS_API extern gpdb Get_Pathfind_Distance_Blocking; // Returns > 0 when succesful. Solves distances immediately. pathObj must be a vehicle or a soldier
SCRIPTS_API extern gmsl Get_Multiplayer_Spawn_Location;
SCRIPTS_API extern esbn Enable_Spawners_By_Name;
SCRIPTS_API extern ipg Is_Pathfind_Generated;
SCRIPTS_API extern gcps Get_Closest_Pathfind_Spot;
SCRIPTS_API extern gcpss Get_Closest_Pathfind_Spot_Size;
SCRIPTS_API extern grcs Get_Radio_Command_String;
SCRIPTS_API extern sei Set_Emot_Icon;
SCRIPTS_API extern kmd Kill_Messages_Disabled;
SCRIPTS_API extern ise Is_Sidebar_Enabled;
SCRIPTS_API extern iee Is_Extras_Enabled;
SCRIPTS_API extern cbg Can_Build_Ground;
SCRIPTS_API extern cba Can_Build_Air;
SCRIPTS_API extern cbn Can_Build_Naval;
SCRIPTS_API extern isb Is_Soldier_Busy;
SCRIPTS_API extern ioep Is_On_Enemy_Pedestal;
SCRIPTS_API extern fcpp Find_Closest_Poly_Position;
SCRIPTS_API extern sdd Say_Dynamic_Dialogue;
SCRIPTS_API extern sddp Say_Dynamic_Dialogue_Player;
SCRIPTS_API extern sddt Say_Dynamic_Dialogue_Team;
SCRIPTS_API extern elp Enable_Letterbox_Player;
SCRIPTS_API extern cst Create_Sound_Team;
SCRIPTS_API extern c2dst Create_2D_Sound_Team;
SCRIPTS_API extern c2dwst Create_2D_WAV_Sound_Team;
SCRIPTS_API extern c2dwstd Create_2D_WAV_Sound_Team_Dialog;
SCRIPTS_API extern c2dwstc Create_2D_WAV_Sound_Team_Cinematic;
SCRIPTS_API extern c3dwsbt Create_3D_WAV_Sound_At_Bone_Team;
SCRIPTS_API extern c3dsbt Create_3D_Sound_At_Bone_Team;
SCRIPTS_API extern _Stop_Sound_Player Stop_Sound_Player;
SCRIPTS_API extern _Stop_Sound_Team Stop_Sound_Team;
SCRIPTS_API extern ssa Set_Subobject_Animation;
SCRIPTS_API extern sts Set_Time_Scale;
SCRIPTS_API extern ssap Set_Subobject_Animation_Player;
SCRIPTS_API extern wfa Write_File_Async;

class SCRIPTS_API JFW_Key_Hook_Base : public ScriptImpClass {
public:
	KeyHookStruct *k;
	int hookid;
	JFW_Key_Hook_Base() : hookid(0)
	{
	}

  /*! Overloaded from ScriptImpClass to call Detach, if a derived class also overrides this that
  class must ensure it calls this base class function in it's own implementation */
  void Detach(GameObject *obj);

  /*! Overloaded from ScriptImpClass to call Detach, if a derived class also overrides this that
  class must ensure it calls this base class function in it's own implementation */
  void Destroyed(GameObject *obj);

  void InstallHook(const char *keyname,GameObject *obj);
	void RemoveHook();
	virtual void KeyHook() = 0;
	~JFW_Key_Hook_Base();
};

/*!
* \brief Script base class with support for multiple key hooks
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This class is a modified variant of JFW_Key_Hook_Base with support for multiple keyhooks being
* installed simultaneously in a single script. Keyhooks are added and removed by name and the
* callback function will now contain the name of the hook that triggered
*/
class SCRIPTS_API MultiKeyHookScriptImpClass : public ScriptImpClass
{
public:
  /*! Overloaded from ScriptImpClass to call Detach, if a derived class also overrides this that
  class must ensure it calls this base class function in it's own implementation */
  void Detach(GameObject *obj);

  /*! Overloaded from ScriptImpClass to call Detach, if a derived class also overrides this that
  class must ensure it calls this base class function in it's own implementation */
  void Destroyed(GameObject *obj);

  /*! Installs a hook on the named logical key for the player who is controlling the specified
  infantry object. It is safe to call this multiple times with the same key name, duplicates will
  not be created */
  void InstallHook(const char* logicalKey, GameObject *obj);

  /*! Removes any hook on the named logical key for the player who is controlling the specified
  infantry object */
  void RemoveHook(const char* logicalKey, GameObject *obj);

  /*! Called when any of the keyhooks bound to the object are triggered */
  virtual void KeyHook(const char* logicalKey) = 0;

  struct KeyHookWrapperStruct
  {
    MultiKeyHookScriptImpClass* owner;
    KeyHookStruct hook;
    int hookId;
  };

protected:
  SList<KeyHookWrapperStruct> ms_keyhooks;

  KeyHookWrapperStruct* FindHook(const char* logicalKey);
};

class SCRIPTS_API JFW_Object_Created_Hook_Base : public ScriptImpClass {
public:
	int hookid;
	JFW_Object_Created_Hook_Base() : hookid(0)
	{
	}
	ObjectCreateHookStruct *h;
	void Detach(GameObject *obj);
	void Created(GameObject *obj);
	void InstallHook();
	void RemoveHook();
	virtual void ObjectCreateHook(GameObject *obj) = 0;
	~JFW_Object_Created_Hook_Base();
};

struct Gap_ListNode 
{
	Gap_ListNode* Next;
	unsigned long unitID;
	float range; 
	int teamID;
	bool active;
};

class ConsoleFunctionClass {
public:
	virtual const char *Get_Name(void) = 0;
	virtual const char *Get_Alias(void) { return NULL; }
	virtual const char *Get_Help(void) { return Get_Name(); }
	virtual void Activate(const char *pArgs) = 0;
	virtual ~ConsoleFunctionClass() { return; }
};

SCRIPTS_API void Kill_All_Buildings_By_Team(int Team); //kill all buildings of the specified team,ending the game with the other team winning,0 = Nod,1 = GDI
SCRIPTS_API void Set_Occupants_Fade(GameObject *obj,float red,float green,float blue,float opacity); //sets the screen fade of all the occupants of a vehicle
SCRIPTS_API void Enable_Team_Radar(int Team,bool Enable); //enable radar for a given team
SCRIPTS_API void Send_Message_Team(int team,unsigned int red,unsigned int green,unsigned int blue,const char *msg); //send a message to a team
SCRIPTS_API void Send_Message_With_Obj_Color(GameObject *obj,const char *Msg); //send a messages in a given objects color
SCRIPTS_API void Send_Message_With_Team_Color(int Team,const char *Msg); //Send a message in a given teams color
SCRIPTS_API void Ranged_Gap_Effect(Gap_ListNode* FirstNode); //apply gap effect to all units of a team within a range
SCRIPTS_API void Ranged_Stealth_On_Team(Gap_ListNode* FirstNode); //Apply stealth to all units in the range of this on the relavent team
SCRIPTS_API bool Can_Team_Build_Vehicle(int Team); //Can this team build vehicles
SCRIPTS_API DynamicVectorClass<int> Get_Enlisted_Purchase_Items(int team);//Get all defined IDs of enlisted characters
SCRIPTS_API DynamicVectorClass<int> Get_Purchase_Items(int team, int defType);//Get all defined IDs in the given purchase list
SCRIPTS_API bool Is_Infantry_Purchaseable(const char *preset,int team); //is this infantry preset purchaseable? (i.e. it would appear on the sidebar and not be hidden or greyed out. Does not do the cost check but does check unit limits)
SCRIPTS_API bool Is_Vehicle_Purchaseable(const char *preset,int team); //is this vehicle preset purchaseable? (i.e. it would appear on the sidebar and not be hidden or greyed out. Does not do the cost check but does check unit limits)
SCRIPTS_API bool Is_Preset_Purchasable_In_List(int id, int team, PurchaseSettingsDefClass::TYPE def); //DA

SCRIPTS_API void SendShaderParam(const char* parameter, const char* value);
SCRIPTS_API void SendShaderParam(const char* parameter, int value);
SCRIPTS_API void SendShaderParam(const char* parameter, unsigned int value);
SCRIPTS_API void SendShaderParam(const char* parameter, float value);
SCRIPTS_API void SendShaderParam(const char* parameter, Vector2 value);
SCRIPTS_API void SendShaderParam(const char* parameter, Vector3 value);
SCRIPTS_API void SendShaderParam(const char* parameter, Vector4 value);
SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, const char* value);
SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, int value);
SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, unsigned int value);
SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, float value);
SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, Vector2 value);
SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, Vector3 value);
SCRIPTS_API void SendShaderParamPlayer(GameObject* player, const char* parameter, Vector4 value);
SCRIPTS_API void SendShaderParamObj(const char* parameter, const char* value, GameObject* object);
SCRIPTS_API void SendShaderParamObj(const char* parameter, int value, GameObject* object);
SCRIPTS_API void SendShaderParamObj(const char* parameter, unsigned int value, GameObject* object);
SCRIPTS_API void SendShaderParamObj(const char* parameter, float value, GameObject* object);
SCRIPTS_API void SendShaderParamObj(const char* parameter, Vector2 value, GameObject* object);
SCRIPTS_API void SendShaderParamObj(const char* parameter, Vector3 value, GameObject* object);
SCRIPTS_API void SendShaderParamObj(const char* parameter, Vector4 value, GameObject* object);
SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, const char* value, GameObject* object);
SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, int value, GameObject* object);
SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, unsigned int value, GameObject* object);
SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, float value, GameObject* object);
SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, Vector2 value, GameObject* object);
SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, Vector3 value, GameObject* object);
SCRIPTS_API void SendShaderParamObjPlayer(GameObject* player, const char* parameter, Vector4 value, GameObject* object);
SCRIPTS_API extern REF_DECL(DynamicVectorClass<ConsoleFunctionClass *>, ConsoleFunctionList);
SCRIPTS_API void Delete_Console_Function(const char *name);
SCRIPTS_API void Sort_Function_List();
SCRIPTS_API void Verbose_Help_File();

/*!
* \brief Send Translated Message To Team(s)
* \ingroup api_translations
*
* Sends the string associated with the specified translation as a message to the specified team and
* optionally also plays the sound associated with the translation (if any) to the team
*
* \param[in] ID
*   The ID of the translation containing the message to be sent
* \param[in] team
*   The team to send the message to, or 2 to send to both teams
* \param[in] red
*   The red component of the colour to use for the message
* \param[in] green
*   The green component of the colour to use for the message
* \param[in] blue
*   The blue component of the colour to use for the message
* \param[in] bPlaySound
*   Whether to also play the sound associated with the translation to the specified team
*/
SCRIPTS_API void Send_Translated_Message_Team ( unsigned long ID, int team, int red, int green, int blue, bool bPlaySound = true );

/*!
* \brief Send Translated Message To Team(s)
* \ingroup api_translations
*
* Sends the string associated with the specified translation as a message to the specified team and
* optionally also plays the sound associated with the translation (if any) to the team
*
* \param[in] ID
*   The ID of the translation containing the message to be sent
* \param[in] team
*   The team to send the message to, or 2 to send to both teams
* \param[in] rgb
*   A vector containing the red, blue and green colour components to use for this message in the X,
*   Y and Z member variables respectively
* \param[in] bPlaySound
*   Whether to also play the sound associated with the translation to the specified team
*/
SCRIPTS_API void Send_Translated_Message_Team ( unsigned long ID, int team, Vector3 rgb, bool bPlaySound = true );

SCRIPTS_API void Force_Vehicle_Entry(GameObject *soldier,GameObject *vehicle);