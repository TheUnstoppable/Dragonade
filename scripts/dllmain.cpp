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
#include "engine.h"
#include "da.h"

#define SCRIPTSAPI __declspec(dllexport)
typedef void (*srdf) (void (*function)(ScriptClass*));
typedef void (*ds) (ScriptClass *);
typedef ScriptClass *(*cs) (const char *);
typedef int (*gsc) ();
typedef char *(*gsn) (int);
typedef char *(*gspd) (int);
typedef bool (*ssc) (ScriptCommandsClass *);
HINSTANCE wwscripts;
cs CreateScript;
ds DestroyScript;
gsc GetScriptCount;
gsn GetScriptName;
gspd GetScriptParamDescription;
srdf SetRequestDestroyFunc;
unsigned int OldCount;
ssc SetScriptCommands;
HMODULE tt;

FARPROC GetProcAddressCheck(HMODULE hModule,LPCSTR lpProcName)
{
	FARPROC x = GetProcAddress(hModule,lpProcName);
	if (!x)
	{
		MessageBox(NULL,"Error getting function address from bandtest.dll",lpProcName,MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST);
	}
	return x;
}
#define Address GetProcAddressCheck
void DestroyExpVehFacClass();

BOOL SCRIPTSAPI __stdcall DllMain(HINSTANCE hinstDLL,
						DWORD	ul_reason_for_call,
						LPVOID	lpReserved
					)
{
	int LastError;
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			TT_ASSERT(Exe != 6);

			/*
			if (Exe != 1)
			{
				MessageBox(HWND_DESKTOP, "Fatal error: Dragonade only works on the FDS. Please replace your scripts.dll with an appropriate version for the Renegade client.", "Renegade", MB_OK | MB_ICONERROR);
				exit(0);
			}
			*/

			if ((!Exe) || (Exe == 1))
			{
				if (GetFileAttributes("scripts2.dll") == 0xFFFFFFFF)
				{
					const char *path2 = Get_File_Path();
					char path[MAX_PATH];
					strcpy(path,path2);
					strcat(path,"dllload.txt");
					FILE *f = fopen(path,"at");
					if (f)
					{
						fprintf(f,"[scripts2.dll] Module not found, unable to continue.\n");
						fclose(f);
					}
					MessageBox(NULL,"scripts2.dll was not found. Are you sure you installed scripts.dll correctly?","Error",MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST);
					ExitProcess(HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND));
				}
				tt = GetModuleHandle("Bandtest.dll");
			}
			wwscripts = LoadLibrary("scripts2.dll");
			LastError = GetLastError();
			if (wwscripts)
			{
				CreateScript = (cs) GetProcAddress(wwscripts,"Create_Script");
				DestroyScript = (ds) GetProcAddress(wwscripts,"Destroy_Script");
				GetScriptCount = (gsc) GetProcAddress(wwscripts,"Get_Script_Count");
				GetScriptName = (gsn) GetProcAddress(wwscripts,"Get_Script_Name");
				GetScriptParamDescription = (gspd) GetProcAddress(wwscripts,"Get_Script_Param_Description");
				SetScriptCommands = (ssc) GetProcAddress(wwscripts,"Set_Script_Commands");
				SetRequestDestroyFunc = (srdf) GetProcAddress(wwscripts,"Set_Request_Destroy_Func");
				OldCount = (GetScriptCount) ();
			}
			else
			{
				const char *path2 = Get_File_Path();
				char path[MAX_PATH];
				strcpy(path,path2);
				strcat(path,"dllload.txt");
				FILE *f = fopen(path,"at");
				if (f)
				{
					char *errorMessage = new char[2048]; //Ugly, but whatever.
					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,LastError,0,errorMessage,2048,NULL);
					fprintf(f,"[scripts2.dll] Failed to load,  error %d. %s\n",LastError,errorMessage);
					delete[] errorMessage;
					fclose(f);
				}
				MessageBox(NULL,"Unfortunately, there was an error loading scripts2.dll. Please report this bug!","Error",MB_OK|MB_ICONEXCLAMATION);
				ExitProcess(LastError);
			}
			break;
		case DLL_PROCESS_DETACH:
			DestroyExpVehFacClass();
			FreeLibrary(wwscripts);
			DA::Shutdown();
			break;
	}
	return TRUE;
}

extern "C"
{
ScriptClass SCRIPTSAPI *Create_Script(const char *ScriptName)
{
	if (!ScriptName)
	{
		return 0;
	}
	ScriptClass *scr = ScriptRegistrar::CreateScript(ScriptName);
	if (scr)
	{
		return scr;
	}
	return (CreateScript) (ScriptName);
}

void SCRIPTSAPI Destroy_Script(ScriptClass *scr)
{
	if (scr)
	{
		delete scr;
	}
}

unsigned int SCRIPTSAPI Get_Script_Count()
{
	int sn = 0;
	sn = ScriptRegistrar::Count();
	return (GetScriptCount) () + sn;
}

const char SCRIPTSAPI *Get_Script_Name(unsigned int Number)
{
	if (Number < OldCount)
	{
		return (GetScriptName) (Number);
	}
	int n = Number - OldCount;
	return ScriptRegistrar::GetScriptFactory(n)->GetName();
}

const char SCRIPTSAPI *Get_Script_Param_Description(unsigned int Number)
{
	if (Number < OldCount)
	{
		return (GetScriptParamDescription) (Number);
	}
	int n = Number - OldCount;
	return ScriptRegistrar::GetScriptFactory(n)->GetParamDescription();
}

bool SCRIPTSAPI Set_Script_Commands(ScriptCommandsClass* commands)
{
	Commands = commands->Commands;
	if ((Commands->Size != sizeof(ScriptCommands)) || (Commands->Version != SCRIPT_COMMANDS_VERSION))
	{
		return false;
	}
	Enable_Stealth_Player = (_Enable_Stealth_Player)Address(tt,"New_Enable_Stealth_Player");
	Set_Fog_Enable_Player = (_Set_Fog_Enable_Player)Address(tt,"New_Set_Fog_Enable_Player");
	Set_Fog_Range_Player = (_Set_Fog_Range_Player)Address(tt,"New_Set_Fog_Range_Player");
	Set_Background_Music_Player = (_Set_Background_Music_Player)Address(tt,"New_Set_Background_Music_Player");
	Fade_Background_Music_Player = (_Fade_Background_Music_Player)Address(tt,"New_Fade_Background_Music_Player");
	Stop_Background_Music_Player = (_Stop_Background_Music_Player)Address(tt,"New_Stop_Background_Music_Player");
	Enable_Radar_Player = (_Enable_Radar_Player)Address(tt,"New_Enable_Radar_Player");
	Display_GDI_Player_Terminal_Player = (_Display_GDI_Player_Terminal_Player)Address(tt,"New_Display_GDI_Player_Terminal_Player");
	Display_NOD_Player_Terminal_Player = (_Display_NOD_Player_Terminal_Player)Address(tt,"New_Display_NOD_Player_Terminal_Player");
	Set_Screen_Fade_Color_Player = (_Set_Screen_Fade_Color_Player)Address(tt,"New_Set_Screen_Fade_Color_Player");
	Set_Screen_Fade_Opacity_Player = (_Set_Screen_Fade_Opacity_Player)Address(tt,"New_Set_Screen_Fade_Opacity_Player");
	Force_Camera_Look_Player = (_Force_Camera_Look_Player)Address(tt,"New_Force_Camera_Look_Player");
	Enable_HUD_Player = (_Enable_HUD_Player)Address(tt,"New_Enable_HUD_Player");
	Create_Sound_Player = (_Create_Sound_Player)Address(tt,"New_Create_Sound_Player");
	Create_2D_Sound_Player = (_Create_2D_Sound_Player)Address(tt,"New_Create_2D_Sound_Player");
	Create_2D_WAV_Sound_Player = (_Create_2D_WAV_Sound_Player)Address(tt,"New_Create_2D_WAV_Sound_Player");
	Create_3D_WAV_Sound_At_Bone_Player = (_Create_3D_WAV_Sound_At_Bone_Player)Address(tt,"New_Create_3D_WAV_Sound_At_Bone_Player");
	Create_3D_Sound_At_Bone_Player = (_Create_3D_Sound_At_Bone_Player)Address(tt,"New_Create_3D_Sound_At_Bone_Player");
	AddObjectCreateHook = (aoch)Address(tt,"NewAddObjectCreateHook");
	RemoveObjectCreateHook = (roch)Address(tt,"NewRemoveObjectCreateHook");
	AddKeyHook = (akh)Address(tt,"AddKeyHook");
	RemoveKeyHook = (rkh)Address(tt,"RemoveKeyHook");
	AddChatHook = (ach)Address(tt,"AddChatHook");
	AddHostHook = (ahh)Address(tt,"AddHostHook");
	AddRefillHook = (arh)Address(tt,"AddRefillHook");
	AddLoadLevelHook = (allh)Address(tt,"AddLoadLevelHook");
	AddPreLoadLevelHook = (allh)Address(tt,"AddPreLoadLevelHook");
	AddGameOverHook = (allh)Address(tt,"AddGameOverHook");
	AddThinkHook = (allh)Address(tt,"AddThinkHook");
	AddPlayerJoinHook = (apjh)Address(tt,"AddPlayerJoinHook");
	AddRadioHook = (arch)Address(tt,"AddRadioHook");
	AddStockDamageHook = (asdh)Address(tt,"AddStockDamageHook");
	AddTtDamageHook = (atdh)Address(tt,"AddTtDamageHook");
	GetCurrentMusicTrack = (gcmt)Address(tt,"GetCurrentMusicTrack");
	Set_Info_Texture = (sit)Address(tt,"Set_Info_Texture");
	Clear_Info_Texture = (cit)Address(tt,"Clear_Info_Texture");
	Set_Vehicle_Limit = (svl)Address(tt,"Set_Vehicle_Limit");
	Get_Vehicle_Limit = (gvl)Address(tt,"Get_Vehicle_Limit");
	Set_Air_Vehicle_Limit = (svl)Address(tt,"Set_Air_Vehicle_Limit");
	Get_Air_Vehicle_Limit = (gvl)Address(tt,"Get_Air_Vehicle_Limit");
	Set_Naval_Vehicle_Limit = (svl)Address(tt,"Set_Naval_Vehicle_Limit");
	Get_Naval_Vehicle_Limit = (gvl)Address(tt,"Get_Naval_Vehicle_Limit");
	Send_Message = (sm)Address(tt,"Send_Message");
	Send_Message_Player = (smp)Address(tt,"Send_Message_Player");
	Load_New_HUD_INI = (lnhi)Address(tt,"Load_New_HUD_INI");
	Remove_Weapon = (rw)Address(tt,"Remove_Weapon");
	Change_Radar_Map = (crm)Address(tt,"Change_Radar_Map");
	AddPowerupPurchaseHook = (aph)Address(tt,"AddPowerupPurchaseHook");
	AddVehiclePurchaseHook = (aph)Address(tt,"AddVehiclePurchaseHook");
	AddCharacterPurchaseHook = (aph)Address(tt,"AddCharacterPurchaseHook");
	AddPowerupPurchaseMonHook = (apmh)Address(tt,"AddPowerupPurchaseMonHook");
	AddVehiclePurchaseMonHook = (apmh)Address(tt,"AddVehiclePurchaseMonHook");
	AddCharacterPurchaseMonHook = (apmh)Address(tt,"AddCharacterPurchaseMonHook");
	RemovePowerupPurchaseHook = (rph)Address(tt,"RemovePowerupPurchaseHook");
	RemoveVehiclePurchaseHook = (rph)Address(tt,"RemoveVehiclePurchaseHook");
	RemoveCharacterPurchaseHook = (rph)Address(tt,"RemoveCharacterPurchaseHook");
	RemovePowerupPurchaseMonHook = (rph)Address(tt,"RemovePowerupPurchaseMonHook");
	RemoveVehiclePurchaseMonHook = (rph)Address(tt,"RemoveVehiclePurchaseMonHook");
	RemoveCharacterPurchaseMonHook = (rph)Address(tt,"RemoveCharacterPurchaseMonHook");
	Get_Build_Time_Multiplier = (gbm)Address(tt,"Get_Build_Time_Multiplier");
	Set_Currently_Building = (scb)Address(tt,"Set_Currently_Building");
	Is_Currently_Building = (icb)Address(tt,"Is_Currently_Building");
	AddConsoleOutputHook = (acoh)Address(tt,"AddConsoleOutputHook");
	AddLogFileHook = (acoh)Address(tt,"AddLogFileHook");
	Set_Fog_Color = (sfc)Address(tt,"New_Set_Fog_Color");
	Set_Fog_Color_Player = (sfcp)Address(tt,"Set_Fog_Color_Player");
	Set_Fog_Mode = (sfm)Address(tt,"New_Set_Fog_Mode");
	Set_Fog_Mode_Player = (sfmp)Address(tt,"Set_Fog_Mode_Player");
	Send_HUD_Number = (shn)Address(tt,"SendHUDNumber");
	Set_Fog_Density = (sfd)Address(tt,"New_Set_Fog_Density");
	Set_Fog_Density_Player = (sfdp)Address(tt,"Set_Fog_Density_Player");
	Change_Time_Remaining = (ctr)Address(tt,"New_Change_Time_Remaining");
	Change_Time_Limit = (ctl)Address(tt,"New_Change_Time_Limit");
	Display_Security_Dialog = (dps)Address(tt,"Display_Security_Dialog");
	AddPlayerLeaveHook = (aplh)Address(tt,"AddPlayerLeaveHook");
	GetExplosionObj = (geo)Address(tt,"GetExplosionObj");
	SetExplosionObj = (seo)Address(tt,"SetExplosionObj");
	Update_Network_Object = (uno)Address(tt,"Update_Network_Object");
	Update_Network_Object_Player = (unop)Address(tt,"Update_Network_Object_Player");
	Send_Client_Text = (sct)Address(tt,"Send_Client_Text");
	Send_Client_Announcement = (sca)Address(tt,"Send_Client_Announcement");
	Send_Player_Kill_Message = (spkm)Address(tt, "Send_Player_Kill_Message");
	Send_Purchase_Response = (spr)Address(tt, "Send_Purchase_Response");
	AddShaderNotify = (asn)Address(tt,"AddShaderNotify");
	RemoveShaderNotify = (rsn)Address(tt,"RemoveShaderNotify");
	Do_Objectives_Dlg = (dod)Address(tt,"Do_Objectives_Dlg");
	Set_Player_Limit = (sl)Address(tt,"Set_Player_Limit");
	Get_Player_Limit = (gl)Address(tt,"Get_Player_Limit");
	Get_Fog_Range = (_Get_Fog_Range)Address(tt,"Get_Fog_Range");
	Send_Shader_Param_Player = (sspp)Address(tt,"Send_Shader_Param_Player");
	Send_Shader_Param = (ssp)Address(tt,"Send_Shader_Param");
	Send_Shader_Param_Obj_Player = (sspop)Address(tt,"Send_Shader_Param_Obj_Player");
	Send_Shader_Param_Obj = (sspo)Address(tt,"Send_Shader_Param_Obj");
	Set_GDI_Soldier_Name = (ssn)Address(tt,"Set_GDI_Soldier_Name");
	Set_Nod_Soldier_Name = (ssn)Address(tt,"Set_Nod_Soldier_Name");
	Get_GDI_Soldier_Name = (gsnx)Address(tt,"Get_GDI_Soldier_Name");
	Get_Nod_Soldier_Name = (gsnx)Address(tt,"Get_Nod_Soldier_Name");
	Get_IP_Address = (Get_IP_Addressx)Address(tt,"Get_IP_Address");
	Get_IP_Port = (Get_IP_Portx)Address(tt,"Get_IP_Port");
	Get_IP = (Get_IPx)Address(tt,"Get_IP");
	Get_Bandwidth = (Get_Bandwidthx)Address(tt,"Get_Bandwidth");
	Get_Ping = (Get_Pingx)Address(tt,"Get_Ping");
	Get_Kbits = (Get_Kbitsx)Address(tt,"Get_Kbits");
	Set_Fog_Override_Player = (sfop)Address(tt,"Set_Fog_Override_Player");
	Clear_Fog_Override_Player = (cfop)Address(tt,"Clear_Fog_Override_Player");
	Display_Game_Hint = (dgh)Address(tt,"Display_Game_Hint");
	Display_Game_Hint_Image = (dghi)Address(tt,"Display_Game_Hint_Image");
	Set_Current_Bullets = (sclb)Address(tt,"Set_Current_Bullets");
	Set_Current_Clip_Bullets = (sclb)Address(tt,"Set_Current_Clip_Bullets");
	Set_Position_Bullets = (spb)Address(tt,"Set_Position_Bullets");
	Set_Position_Clip_Bullets = (spb)Address(tt,"Set_Position_Clip_Bullets");
	Set_Bullets = (sb)Address(tt,"Set_Bullets");
	Set_Clip_Bullets = (sb)Address(tt,"Set_Clip_Bullets");
	Set_Moon_Is_Earth = (smie)Address(tt,"Set_Moon_Is_Earth");
	Get_Mine_Limit = (gml)Address(tt,"Get_Mine_Limit");
	Set_Special_Base_Destruction = (sbd)Address(tt,"Set_Special_Base_Destruction");
	Get_Client_Version = (gpv)Address(tt,"Get_Client_Version");
	Get_Client_Revision = (gpr)Address(tt,"Get_Client_Revision");
	Get_Revision = (gpr)Address(tt,"Get_Revision");
	Get_Damage_Warhead = (gdw)Address(tt,"Get_Damage_Warhead");
	Get_Damage_Is_Explosion = (gdie)Address(tt, "Get_Damage_Is_Explosion");
	Get_GameObj_By_Player_Name = (ggobpn)Address(tt,"Get_GameObj_By_Player_Name");
	Send_Custom_All_Players = (scap)Address(tt,"Send_Custom_All_Players");
	Steal_Team_Credits = (stc)Address(tt,"Steal_Team_Credits");
	Get_Team_Credits = (gtc)Address(tt,"Get_Team_Credits");
	addConnectionAcceptanceFilter = (addConnectionAcceptanceFilterType)Address(tt, "addConnectionAcceptanceFilter");
	removeConnectionAcceptanceFilter = (removeConnectionAcceptanceFilterType)Address(tt, "removeConnectionAcceptanceFilter");
	Is_WOL_User = (iwu)Address(tt, "Is_WOL_User");
	Evict_Client = (ec)Address(tt,"Evict_Client");
	Get_Current_Map_Index = (gcmi)Address(tt,"Get_Current_Map_Index");
	Get_Player_List = (gpl)Address(tt,"Get_Player_List");
	Get_Map = (gm)Address(tt,"Get_Map");
	Get_Map_Count = (gmc)Address(tt,"Get_Map_Count");
	Set_Map = (smx)Address(tt,"Set_Map");
	Get_Game_Definitions = (ggd)Address(tt,"Get_Game_Definitions");
	Get_Client_Serial_Hash = (gcsh)Address(tt,"Get_Client_Serial_Hash");
	Get_Tech_Level = (gml)Address(tt,"Get_Tech_Level");
	Set_Tech_Level = (sw)Address(tt,"Set_Tech_Level");
	Create_Explosion_Extended = (cee)Address(tt,"Create_Explosion_Extended");
	Retrieve_Waypaths = (rwpa)Address(tt,"Retrieve_Waypaths");
	Retrieve_Waypoints = (rwpo)Address(tt,"Retrieve_Waypoints");
	Get_Waypoint_Position = (gwp)Address(tt,"Get_Waypoint_Position");
	Test_Raycast_Collision = (trc)Address(tt,"Test_Raycast_Collision");
	Is_Inside_AABox = (iib)Address(tt,"Is_Inside_AABox");
	Can_Generic_Soldier_Teleport = (cgst)Address(tt,"Can_Generic_Soldier_Teleport");
	Create_Lightning = (cl)Address(tt,"Create_Lightning");
	Set_Global_Stealth_Disable = (smie)Address(tt,"Set_Global_Stealth_Disable");
	Get_Clouds = (gc) Address(tt, "Get_Clouds");
	Get_Lightning = (gli) Address(tt, "Get_Lightning");
	Get_Wind = (gw) Address(tt, "Get_Wind");
	Get_Rain = (gp) Address(tt, "Get_Rain");
	Get_Snow = (gp) Address(tt, "Get_Snow");
	Get_Ash = (gp) Address(tt, "Get_Ash");
	Update_Game_Options = (ugo) Address(tt, "Update_Game_Options");
	GetTTVersion = (gttv)Address(tt,"GetTTVersion");
	GetTTRevision = (gttr)Address(tt,"GetTTRevision");
	Set_HUD_Help_Text_Player = (htp)Address(tt,"Set_HUD_Help_Text_Player");
	Change_String_Player = (csp)Address(tt,"Change_String_Player");
	Force_Position_Update = (fpu)Address(tt,"Force_Position_Update");
	Get_Random_Pathfind_Spot = (grps)Address(tt,"Get_Random_Pathfind_Spot");
	Set_HUD_Help_Text_Player_Text = (htpt)Address(tt,"Set_HUD_Help_Text_Player_Text");
	Grant_Weapon = (grw)Address(tt,"Grant_Weapon");
	Set_Object_Visibility_For_Player = (sovfp)Address(tt,"Set_Object_Visibility_For_Player");
	Set_Object_Visibility = (sov)Address(tt,"Set_Object_Visibility");
	Lock_Soldier_Collision_Group = (lscg)Address(tt,"Lock_Soldier_Collision_Group");
	Unlock_Soldier_Collision_Group = (ulscg)Address(tt,"Unlock_Soldier_Collision_Group");
	Is_Engine_Enabled = (iea)Address(tt,"Is_Engine_Enabled");
	Stop_Timer = (ss)Address(tt,"Stop_Timer");
	Create_2D_Wave_Sound_Dialog_Player = (cwsdp)Address(tt,"Create_2D_Wave_Sound_Dialog_Player");
	Force_Position_Update_Player = (fpup)Address(tt,"Force_Position_Update_Player");
	Stop_Timer2 = (ss2)Address(tt,"Stop_Timer2");
	Has_Timer = (htm)Address(tt,"Has_Timer");
	Set_Background_Music_Player_Offset = (sbgmo)Address(tt,"Set_Background_Music_Player_Offset");
	Set_Camera_Player = (setcam)Address(tt,"Set_Camera_Player");
	Set_Definition_TranslationID_Player = (settran)Address(tt,"Set_Definition_TranslationID_Player");
	Set_Net_Update_Rate_Player = (setnur)Address(tt,"Set_Net_Update_Rate_Player");
	Force_Orientation_Update = (fou)Address(tt,"Force_Orientation_Update");
	Force_Orientation_Update_Player = (foup)Address(tt,"Force_Orientation_Update_Player");
	Create_2D_Wave_Sound_Cinematic_Player = (cwscp)Address(tt,"Create_2D_Wave_Sound_Cinematic_Player");
	Force_Turret_Update = (ftu)Address(tt,"Force_Turret_Update");
	Force_Turret_Update_Player = (ftup)Address(tt,"Force_Turret_Update_Player");
	Force_Velocity_Update = (fvu)Address(tt,"Force_Velocity_Update");
	Force_Velocity_Update_Player = (fvup)Address(tt,"Force_Velocity_Update_Player");
	Set_Camera_Host_Network = (schn)Address(tt,"Set_Camera_Host_Network");
	Get_Pathfind_Distance = (gpd)Address(tt,"Get_Pathfind_Distance");
	Cancel_Get_Pathfind_Distance = (cgpd)Address(tt,"Cancel_Get_Pathfind_Distance");
	Get_Pathfind_Distance_Async = (gpda)Address(tt, "Get_Pathfind_Distance_Async");
	Get_Pathfind_Distance_Blocking = (gpdb)Address(tt, "Get_Pathfind_Distance_Blocking");
	Get_Multiplayer_Spawn_Location = (gmsl)Address(tt, "Get_Multiplayer_Spawn_Location");
	Enable_Spawners_By_Name = (esbn)Address(tt, "Enable_Spawners_By_Name");
	Is_Pathfind_Generated = (ipg)Address(tt, "Is_Pathfind_Generated");
	Get_Closest_Pathfind_Spot = (gcps)Address(tt, "Get_Closest_Pathfind_Spot");
	Get_Closest_Pathfind_Spot_Size = (gcpss)Address(tt, "Get_Closest_Pathfind_Spot_Size");
	Get_Radio_Command_String = (grcs)Address(tt, "Get_Radio_Command_String");
	Set_Emot_Icon = (sei)Address(tt, "Set_Emot_Icon");
	Kill_Messages_Disabled = (kmd)Address(tt, "Kill_Messages_Disabled");
	Is_Sidebar_Enabled = (ise)Address(tt, "Is_Sidebar_Enabled");
	Is_Extras_Enabled = (iee)Address(tt, "Is_Extras_Enabled");
	Can_Build_Ground = (cbg)Address(tt, "Can_Build_Ground");
	Can_Build_Air = (cba)Address(tt, "Can_Build_Air");
	Can_Build_Naval = (cbn)Address(tt, "Can_Build_Naval");
	Is_Soldier_Busy = (isb)Address(tt, "Is_Soldier_Busy");
	Is_On_Enemy_Pedestal = (ioep)Address(tt, "Is_On_Enemy_Pedestal");
	Find_Closest_Poly_Position = (fcpp)Address(tt, "Find_Closest_Poly_Position");
	Say_Dynamic_Dialogue = (sdd)Address(tt, "Say_Dynamic_Dialogue");
	Say_Dynamic_Dialogue_Player = (sddp)Address(tt, "Say_Dynamic_Dialogue_Player");
	Say_Dynamic_Dialogue_Team = (sddt)Address(tt, "Say_Dynamic_Dialogue_Team");
	Enable_Letterbox_Player = (elp)Address(tt, "Enable_Letterbox_Player");
	Create_Sound_Team = (cst)Address(tt, "Create_Sound_Team");
	Create_2D_Sound_Team = (c2dst)Address(tt, "Create_2D_Sound_Team");
	Create_2D_WAV_Sound_Team = (c2dwst)Address(tt, "Create_2D_WAV_Sound_Team");
	Create_2D_WAV_Sound_Team_Dialog = (c2dwstd)Address(tt, "Create_2D_WAV_Sound_Team_Dialog");
	Create_2D_WAV_Sound_Team_Cinematic = (c2dwstc)Address(tt, "Create_2D_WAV_Sound_Team_Cinematic");
	Create_3D_WAV_Sound_At_Bone_Team = (c3dwsbt)Address(tt, "Create_3D_WAV_Sound_At_Bone_Team");
	Create_3D_Sound_At_Bone_Team = (c3dsbt)Address(tt, "Create_3D_Sound_At_Bone_Team");
	Stop_Sound_Player = (_Stop_Sound_Player)Address(tt, "Stop_Sound_Player");
	Stop_Sound_Team = (_Stop_Sound_Team)Address(tt, "Stop_Sound_Team");
	Set_Subobject_Animation = (ssa)Address(tt,"Set_Subobject_Animation");
	Set_Time_Scale = (sts)Address(tt,"Set_Time_Scale");
	Set_Subobject_Animation_Player = (ssap)Address(tt,"Set_Subobject_Animation_Player");
	Write_File_Async = (wfa)Address(tt, "Write_File_Async");

	DA::Init();

	return (SetScriptCommands) (commands);
}

void SCRIPTSAPI Set_Request_Destroy_Func(void (*function)(ScriptClass*))
{
	ScriptImpClass::Set_Request_Destroy_Func(function);
	(SetRequestDestroyFunc)(function);
}
} //extern "C"
