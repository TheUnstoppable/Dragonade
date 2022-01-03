/*	Renegade Scripts.dll
	Copyright 2017 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once
class SCUD_Turret_Spawn : public ScriptImpClass {
	int turID;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class SCUD_Harvester_Zone: public ScriptImpClass {
	bool enabled;
	void Timer_Expired(GameObject *obj,int number);
	void Entered(GameObject *obj,GameObject *enterer);
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class SCUD_Custom_Follow_Waypath : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
};

class SCUD_Follow_Waypath_Until_Custom : public ScriptImpClass {
	bool disable;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	public: void Register_Auto_Save_Variables();
};

class SCUD_Poke_Preset_Buy : public ScriptImpClass {
	int active;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	public: void Register_Auto_Save_Variables();
};

class SCUD_Preset_Buy : public ScriptImpClass {
	int pilotID;
	int active;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class SCUD_CustomSend_Preset_Buy : public ScriptImpClass {
	int pilotID;
	int active;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class SCUD_CustomSend_PresetBuy_FollowWaypath : public ScriptImpClass {
	int pilotID;
	int active;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class SCUD_CustomSend_PresetBuy_GeneralPresets_FollowWaypath : public ScriptImpClass {
	int pilotID;
	int active;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class SCUD_Deployable_Vehicle : public ScriptImpClass {
	int pilotID;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class SCUD_Deploy_Animation : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class SCUD_DeathZoneOnCustom : public ScriptImpClass {
	int visitor;
	void Entered(GameObject *obj,GameObject *enterer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Exited(GameObject *obj,GameObject *exiter);

};

class SCUD_Harvester_Control : public ScriptImpClass {
	int USHarvLim;
	int CHHarvLim;
	int chlimreacht;
	int uslimreacht;
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class SCUD_CHHarvester : public ScriptImpClass {
	int HarvCtrlID;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class SCUD_USHarvester : public ScriptImpClass {
	int HarvCtrlID;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class SCUD_USHarvester_Terminal : public ScriptImpClass {
	int pilotID;
	int active;
	int visitorID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class SCUD_CHHarvester_Terminal : public ScriptImpClass {
	int pilotID;
	int active;
	int visitorID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class SCUD_Set_Player_Type_OnCustom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class SCUD_SuperWeapon : public ScriptImpClass {
	int counter;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class SCUD_DozerBuildTurret : public ScriptImpClass {
	int PilotID;
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class SCUD_TurretBuildup : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};
