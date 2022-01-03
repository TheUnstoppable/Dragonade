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
class JFW_User_Controllable_Base_Defence : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool player;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Base Defence
* \author jonwil
* \ingroup scripts_basedefences
*
* A simple base defence script that will attack any enemy object that enters its firing range
*
* \param MinAttackDistance
*   Minimum distance at which this defence will target enemies
* \param MaxAttackDistance
*   Maximum distance at which this defence will target enemies. Note that the sight range of the
*   preset still determines the distance at which it will see enemies.
* \param AttackTimer
*   Time, in seconds, before the attack action expires. If an enemy is still in range a new attack
*   action will be triggered when the Enemy_Seen function fires.
*/
class JFW_Base_Defence : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence that will not attack objects of up to 8 different preset types.
* Note that despite the name the list of presets to avoid attacking is not limited to aircraft only.
*
* \note
*   If you simply want a base defence script that does not target VTOL objects without having to
*   manually specify each preset you could use JFW_Base_Defence_No_VTOL instead.
*
* \todo Document script parameters
*/
class JFW_Base_Defence_No_Aircraft : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence that will only attack objects of up to 8 different preset types.
* Note that despite the name the list of presets to attack is not limited to aircraft only.
*
* \note
*   If you simply want a base defence script that only targets VTOL objects without having to
*   manually specify each preset you could use JFW_Base_Defence_VTOL_Only instead.
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Aircraft_Only : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Base Defence (Secondary Fire)
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence that will use secondary fire when attacking enemies
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Secondary : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_No_Aircraft that will use secondary fire when attacking enemies
*
* \note
*   If you simply want a base defence script that does not target VTOL objects without having to
*   manually specify each preset you could use JFW_Base_Defence_No_VTOL_Secondary instead.
*
* \todo Document script parameters
*/
class JFW_Base_Defence_No_Aircraft_Secondary : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_Aircraft_Only that will use secondary fire when attacking enemies
*
* \note
*   If you simply want a base defence script that does only targets VTOL objects without having to
*   manually specify each preset you could use JFW_Base_Defence_VTOL_Only_Secondary instead.
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Aircraft_Only_Secondary : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Popup Base Defence
* \author jonwil
* \ingroup scripts_basedefences
*
* A popup base defence script that plays a popup animation prior to firing upon an enemy and replays
* the same animation in reverse once it no longer has any targets to attack
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Animated : public ScriptImpClass {
	bool popup;
	bool attack;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_Animated that will not attack objects of up to 8 different preset
* types. Note that despite the name the list of presets to avoid attacking is not limited to
* aircraft only.
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Animated_No_Aircraft : public ScriptImpClass {
	bool popup;
	bool attack;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_Animated that will only attack objects of up to 8 different preset
* types. Note that despite the name the list of presets to attack is not limited to aircraft only.
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Animated_Aircraft_Only : public ScriptImpClass {
	bool popup;
	bool attack;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Popup Base Defence (Secondary Fire)
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_Animated that will use secondary fire when attacking enemies
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Animated_Secondary : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_Animated_No_Aircraft that will use secondary fire when attacking
* enemies
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Animated_No_Aircraft_Secondary : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_Animated_Aircraft_Only that will use secondary fire when attacking
* enemies
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Animated_Aircraft_Only_Secondary : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Ground Only Base Defence
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence that will not attack VTOL objects
*
* \todo Document script parameters
*/
class JFW_Base_Defence_No_VTOL : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence that will not attack VTOL or stealth objects
*
* \todo Document script parameters
*/
class JFW_Base_Defence_No_VTOL_No_Stealth : public JFW_Base_Defence_No_VTOL {
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
};

/*!
* \brief Anti VTOL Base Defence
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence that will only attack VTOL objects
*
* \todo Document script parameters
*/
class JFW_Base_Defence_VTOL_Only : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Ground Only Base Defence (Secondary Fire)
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_No_VTOL that will use secondary fire when attacking enemies
*
* \todo Document script parameters
*/
class JFW_Base_Defence_No_VTOL_Secondary : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Anti VTOL Base Defence (Secondary Fire)
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_VTOL_Only that will use secondary fire when attacking enemies
*
* \todo Document script parameters
*/
class JFW_Base_Defence_VTOL_Only_Secondary : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Ground Only Popup Base Defence
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_Animated that will not attack VTOL objects
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Animated_No_VTOL : public ScriptImpClass {
	bool popup;
	bool attack;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Anti VTOL Popup Base Defence
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_Animated that will only attack VTOL objects
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Animated_VTOL_Only : public ScriptImpClass {
	bool popup;
	bool attack;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Ground Only Popup Base Defence (Secondary Fire)
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_Animated that will not attack VTOL objects and will use secondary
* fire when attacking enemies
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Animated_No_VTOL_Secondary : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

/*!
* \brief Anti VTOL Popup Base Defence (Secondary Fire)
* \author jonwil
* \ingroup scripts_basedefences
*
* A variant of JFW_Base_Defence_Animated that will only attack VTOL objects and will use secondary
* fire when attacking enemies
*
* \todo Document script parameters
*/
class JFW_Base_Defence_Animated_VTOL_Only_Secondary : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound : public ScriptImpClass {
	bool popup;
	bool attack;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_No_Aircraft : public ScriptImpClass {
	bool popup;
	bool attack;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_Aircraft_Only : public ScriptImpClass {
	bool popup;
	bool attack;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_Secondary : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_No_Aircraft_Secondary : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_Aircraft_Only_Secondary : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_No_VTOL : public ScriptImpClass {
	bool popup;
	bool attack;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_VTOL_Only : public ScriptImpClass {
	bool popup;
	bool attack;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_No_VTOL_Secondary : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_VTOL_Only_Secondary : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Swap : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_No_Aircraft_Swap : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Aircraft_Only_Swap : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Swap : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_No_Aircraft_Swap : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Aircraft_Only_Swap : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_No_VTOL_Swap : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_VTOL_Only_Swap : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	unsigned int objtype;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_No_VTOL_Swap : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_VTOL_Only_Swap : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_Swap : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_No_Aircraft_Swap : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_Aircraft_Only_Swap : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_No_VTOL_Swap : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Base_Defence_Animated_Sound_VTOL_Only_Swap : public ScriptImpClass {
	bool popup;
	bool attack;
	bool primary;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};
