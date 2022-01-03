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
class JFW_Custom_Toggle_Radar : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Reflect_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Reflect_Custom_Delay : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Custom_Destroy_Object : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Custom_Objectives_Dlg : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

/*!
* \brief Send Custom on Death
* \author jonwil
* ingroup scripts_customgenerators
*
* Script to send a custom message when the object it is attached to is killed
*
* \param ID
*   ID of the object to send the custom message to
* \param Message
*   The type of custom message to send
* \param Param
*   The parameter to send with the custom message
*/
class JFW_Death_Send_Custom : public ScriptImpClass {
  void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Custom_Destroy_Building : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Timer_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_2D_Sound_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_3D_Sound_Custom_2 : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_3D_Sound_Custom_Bone : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Random_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Custom_Damage_Object : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Blow_Up_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Blow_Up_Obj_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Delay_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Startup_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Teleport_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Teleport_Custom_Random : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Teleport_Custom_Timer : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Blow_Up_Location_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_3D_Sound_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Created_Send_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Damaged_Send_Custom : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class JFW_Enemy_Seen_Send_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
};

class JFW_Create_Object_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Enable_Spawner_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Animation_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Model_Animation_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Model_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Enable_Disable_Animation_On_Custom : public ScriptImpClass {
	bool enabled;
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
};

class JFW_Custom_Toggle_Visible : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Customs_Send_Custom : public ScriptImpClass {
	bool custom1;
	bool custom2;
	bool custom3;
	bool custom4;
	bool custom5;
	bool custom6;
	bool custom7;
	bool custom8;
	bool trigger;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Custom_Multiple_Send_Custom : public ScriptImpClass {
	int times_recieved;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Custom_Multiple_Send_Custom_2 : public ScriptImpClass {
	int times_recieved;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Custom_Send_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Custom_Send_Random_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Create_Destroy_Object_On_Custom : public ScriptImpClass {
	int ObjectID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Create_Destroy_Object_On_Custom_Animate : public ScriptImpClass {
	int ObjectID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Create_Destroy_Object_On_Custom_Animate_2 : public ScriptImpClass {
	int ObjectID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Damaged_Send_Custom_Amount : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class JFW_Invulnerable_On_Custom : public ScriptImpClass {
	bool enabled;
	float health;
	float shield_strength;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	public: void Register_Auto_Save_Variables();
};

class JFW_Invulnerable_On_Custom_2 : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Custom_Send_Custom_Parameter : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Create_Destroy_Object_On_Custom_Bone : public ScriptImpClass {
	int ObjectID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Custom_Send_Ordered_Customs : public ScriptImpClass {
	int CurrentCustom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Custom_Send_Customs : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Custom_Send_Custom_Multiple_IDs : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Create_Destroy_Object_On_Custom_Death : public ScriptImpClass {
	int ObjectID;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Create_Destroy_Object_On_Custom_Death_Bone : public ScriptImpClass {
	int ObjectID;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Custom_Destroy_All_Buildings : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Timer_Custom_On_Off : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Create_Vehicle_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Model_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Death_Send_Custom_ID : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Death_Send_Custom_Driver_ID : public ScriptImpClass {
	int DriverID;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Repair_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

/*!
* \brief Set wind conditions on custom
* \author jonwil
* \ingroup scripts_weather
*/
class JFW_Wind_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Set war blitz lightning conditions on custom
* \author jonwil
* \ingroup scripts_weather
*/
class JFW_War_Blitz_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Set fog conditions on custom
* \author jonwil
* \ingroup scripts_weather
*/
class JFW_Fog_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Set lightning conditions on custom
* \author jonwil
* \ingroup scripts_weather
*/
class JFW_Lightning_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Set precipitation conditions on custom
* \author jonwil
* \ingroup scripts_weather
*/
class JFW_Precipitation_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Set cloud conditions on custom
* \author jonwil
* \ingroup scripts_weather
*/
class JFW_Clouds_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Enable_Stealth_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Console_Message_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Console_Input_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Invulnerable_On_Custom_3 : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Skin_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Armor_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Change_Team_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Enable_Team_Radar_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_2D_Sound_Team_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_3D_Sound_Team_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Send_Custom_Distance_Objects_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Send_Custom_Distance_Objects_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Send_Custom_All_Objects_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Send_Custom_All_Objects_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Destroy_Create_Object_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Send_Message_Preset : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

class JFW_Shader_Custom_Zone_Entry : public ScriptImpClass {
};

class JFW_Shader_Custom_Zone_Exit : public ScriptImpClass {
};

class JFW_Shader_Custom_Vehicle_Entry : public ScriptImpClass {
};

class JFW_Shader_Custom_Vehicle_Exit : public ScriptImpClass {
};

class JFW_Shader_Custom_Create : public ScriptImpClass {
};

class JFW_Shader_Custom_Poke : public ScriptImpClass {
};

class JFW_Kill_All_Buildings_Customs : public ScriptImpClass {
	int count;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Send_Self_Custom_On_Key : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class JFW_Send_Message_On_Key : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

/*!
* \brief Send a custom to itself
* \author jonwil
* \ingroup scripts_customgenerators
*
* Attach this script to an object to make it send a custom message to itself at the point at which
* this script is attached. The custom can optionally be delayed by a set number of seconds.
*
* \param Message
*   The ID of the custom message to send
* \param Param
*   The parameter value to be sent with the custom message
* \param Delay
*   The number of seconds to wait before sending the custom message
*/
class JFW_Startup_Custom_Self : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief Send a custom based on health values
* \author jonwil
* \ingroup scripts_customgenerators
*
* Attach this to an object with health. When the script starts up, it sets a flag to "above". 
* When the object is damaged, it checks the value of "current health" + "current shield strength" 
* against the "Health" parameter. If "current health" + "current shield strength" is less than the
* "Health" parameter and the "above" flag is set, it clears the "above" flag and sends the custom
* specified by "Message1" to the object with "ID1". If "current health" + "current shield strength"
* is greater than the "Health" parameter and the "above" flag is not set, it sets the "above" flag
* and sends the custom specified by "Message2" to the object with "ID2".
*
* \param Message1
*   Message to send when the objects health+shield goes below the set value
* \param ID1
*   ID of the object to send Message1 to
* \param Message2
*   Message to send when the objects health+shield goes above the set value
* \param ID2
*   ID of the object to send Message2 to
* \param Health
*   Health value to check against
*/
class JFW_Send_Custom_On_Damage : public ScriptImpClass {
	bool above;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};
