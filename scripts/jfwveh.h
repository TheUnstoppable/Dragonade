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
class JFW_Aircraft_Fuel : public ScriptImpClass {
	bool entered;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Turret_Spawn_2 : public ScriptImpClass {
	int turID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};


/*!
* \brief Vehicle Turret Spawner Variant (Enter/Exit Customs & Health Link)
* \author jonwil, Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup custom_generator
*
* This is a variant of the basic JFW_Turret_Spawn_2 script with the added ability to send custom
* messages to the turret object when a driver enters or exits the vehicle. Additionally it links
* the health of the turrets with that of the vehicle body so any damage taken is applied to both.
*
* \param Turret_Preset
*   Name of the preset to be spawned
* \param Bone_Name
*   Name of the bone on the object to attach the spawned preset to
* \param Explosion_Preset
*   Name of an explosion preset to create when the object is destroyed, this is used to destroy
*   the spawned turret
* \param Explosion_Bone
*   Name of the bone on the object to create the explosion at
* \param Driver_Entered_Custom
*   ID of the custom message to be sent when a driver enters the vehicle
* \param Driver_Exited_Custom
*   ID of the custom message to be sent when the driver leaves the vehicle
*
* \note
*   The Driver_Exited_Custom custom will be sent to the spawned turret as soon as it is created
*/
class JFW_Turret_Spawn_3 : public ScriptImpClass
{
  int turID;
  bool m_bHasDriver;
  void Created(GameObject *obj);
  void Custom(GameObject *obj,int type,int param,GameObject *sender);
  void Killed(GameObject *obj,GameObject *killer);
  public: void Register_Auto_Save_Variables();
};

class JFW_Drive_To_Player : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Vechicle_Animation : public ScriptImpClass {
	bool enabled;
	float xpos;
	float ypos;
	float zpos;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
	void Animation_Complete ( GameObject *obj, const char *animation_name );
};

class JFW_Visible_Person_In_Vechicle : public ScriptImpClass {
	int modelid;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class JFW_Visible_People_In_Vechicle : public ScriptImpClass {
	int modelid1;
	int modelid2;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class JFW_Per_Preset_Visible_Person_In_Vechicle : public ScriptImpClass {
	int modelid;
	bool stealth;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Per_Preset_Visible_People_In_Vechicle : public ScriptImpClass {
	int modelid1;
	int modelid2;
	int driverid1;
	int driverid2;
	bool stealth;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Visible_Person_Settings : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Visible_Person_Settings_2 : public ScriptImpClass {
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
};

class JFW_Vechicle_Animation_2 : public ScriptImpClass {
	bool enabled;
	float xpos;
	float ypos;
	float zpos;
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Vehicle_Model_Team : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Model_Preset : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Model_Weapon : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vechicle_Animation_Trigger : public ScriptImpClass {
	bool enabled;
	float xpos;
	float ypos;
	float zpos;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Animation_Complete ( GameObject *obj, const char *animation_name );
};

class JFW_Vehicle_Block_Preset : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Extra : public ScriptImpClass {
	int turID;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	public: void Register_Auto_Save_Variables();
};

class JFW_Vehicle_Extra_Attach : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class JFW_Vehicle_Extra_2 : public ScriptImpClass {
	int turID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_Vehicle_Lock : public ScriptImpClass {
	int OwnerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Vehicle_Effect_Animation : public ScriptImpClass {
	bool enabled;
	int count;
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	void Animation_Complete ( GameObject *obj, const char *animation_name );
};

class JFW_Vehicle_Regen_2 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Regen_3 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Aircraft_Refill : public ScriptImpClass {
	bool active;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Aircraft_Refill_Animation : public ScriptImpClass {
	bool active;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Vehicle_Reinforcement : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Empty_Vehicle_Timer : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Vehicle_Visible_Weapon : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class SoldierObject : public NoEqualsClass<SoldierObject>
{
public:
	int ModelID;
	int SoldierID;
	SoldierObject() : ModelID(0), SoldierID(0)
	{
	}
};

class JFW_Per_Preset_Visible_Multiple_People_In_Vehicle : public ScriptImpClass {
	VectorClass<SoldierObject> SoldierObjects;
	bool stealth;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj, int number);
};

class JFW_Per_Preset_Visible_Multiple_People_In_Vehicle_2 : public ScriptImpClass {
	VectorClass<SoldierObject> SoldierObjects;
	bool stealth;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj, int number);
};
