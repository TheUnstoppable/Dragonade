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
class TDA_Stealth_Armor:public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int timer_id);
};

// Use this to send messages on ZoneEnter and Zone Exit
//
//TDA_Send_Custom_Zone
//ID:int					'id of object to send to 
//EnterMessage:int		'message to send on ZoneEnter
//EnterParam:int			'Parameter on ZoneEnter
//ExitMessage:int			'message to send on Exited
//ExitParam:int			'Parameter on Exited
//Team_ID:int			'Team ID 0=Nod,1=GDI,2=Any

class TDA_Send_Custom_Zone: public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

// Construction Yard Controllers,will send the appropriate messages to the specified objects
// to make them QUIT repairing the buildings
//
//TDA_Conyard_Controller
//
//Building1_ID=0:int		'1st building to disable
//Building2_ID=0:int		'2nd building to disable
//Building3_ID=0:int		'3rd building to disable
//Building4_ID=0:int		'4th building to disable
//Building5_ID=0:int		'5th building to disable
//Building6_ID=0:int		'6th building to disable
//Building7_ID=0:int		'7th building to disable
//Building8_ID=0:int		'8th building to disable
//Building9_ID=0:int		'9th building to disable
//Building10_ID=0:int		'10th building to disable

class TDA_Conyard_Controller: public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enable_Repairs(GameObject *obj,bool Enable);
};

// Conyard Repair Script,still working on the script commands to do this,but for now
// will act as a placeholder for the script. Use the controller script to turn off.
//
// TDA_Conyard_Repair
// Repair_Frequency:int		'how many ticks till it recieves 1 health
// Timer_ID:int				'ID of the object it is attached to,will crash game if left blank.

class TDA_Conyard_Repair: public ScriptImpClass {
	void Timer_Expired(GameObject *obj,int timer_id);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	bool Enabled;
};

// Use this to get the flying objects to appear on the Helipad...
//
//TDA_User_Purchased_VTOL_Object
//Preset_Name:string		'flying objects name
//HelipadLocation:Vector	'location to create the object

class TDA_User_Purchased_VTOL_Object:public ScriptImpClass {
	void Created(GameObject *obj);
};

// This will disable helicopter purchases upon destruction
//
//TDA_Helipad_Controller
//VTOL_Controller1=0:int		VTOL Controller to kill on destruction
//VTOL_Controller2=0:int		VTOL Controller to kill on destruction
//VTOL_Controller1=0:int		VTOL Controller to kill on destruction
//VTOL_Controller2=0:int		VTOL Controller to kill on destruction

class TDA_Helipad_Controller:public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

// This is the VTOL_Controller that actually creates the Helicopters

//TDA_VTOL_Controller
//Cinematic_Object:string		text cinematic object to create the helicopter

class TDA_VTOL_Controller:public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

// This is used to grab the purchase from the PT and then send a request to build it 
//to the VTOL Controller
//
//TDA_VTOL_Object
//VTOL_Controller=0:int		ID of the VTOL Controller to bind to.

class TDA_VTOL_Object:public ScriptImpClass {
	void Created(GameObject *obj);
};

// Use this to enable/disable stealth effect. Will activate stealth on enter,and deactivate stealth on exit
//
//TDA_Toggle_Stealth_Zone
//Trigger_Type:int			This is a new setting,you can set the following here.
//							0=only stealth IN zone
//							1=zone entry to enable
//							2=zone exit to disable
//Player_Type				Same as others 0=Nod,1=GDI,2=Any

class TDA_Toggle_Stealth_Zone:public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

// use this to teleport a unit to a desired location when they enter this zone 
//	facing the same direction as when they entered the zone. Either use the Location OR the Object_ID.
//
//TDA_Teleport_Zone
//Location					Vector location of where to be sent
//Object_ID					Object to teleport to (use an editor only object for this,or else will teleport inside
//							the other object

class TDA_Teleport_Zone:public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

// This will Disable a building on Entered,it will not destroy,just disable.
//
// TDA_Disable_Building_Zone
// Building_ID:int			ID of the building to disable
// Team_ID:int				ID of the team to trigger the script (0=Nod,1=GDI,2=Any)

class TDA_Disable_Building_Zone:public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

// These are the controller scripts for a stealth generator attached to a Zone. 
// when player x is in zone y,stealth is enabled,when they leave the zone,it is disabled.
//
//TDA_Stealth_Generator
//Zone_ID:int				ID of the Connecting Zone

class TDA_Stealth_Generator:public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

//Zone Script for above
//
//TDA_Stealth_Generator_Zone
//Player_Type:int			player type (0=Nod,1=GDI,2=Any (unteamed))

class TDA_Stealth_Generator_Zone:public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	bool Disabled;
};

//This is a Script use only script,no practical general use for it
//
//TDA_CTF_Attached
//No Parameters

class TDA_CTF_Attached:public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	unsigned int MyFlagId;
	unsigned int ZoneCapturedId;
};

//Attach this to a zone for CTF type game... (for this to work,the game 
//			HAS to be able to end on Destroy Buildings)
//
//TDA_CTF_Zone
//Team_ID=0:int					This is what team the Script Zone works for (0=Nod,1=GDI)
//Max_Capture=5:int				This is the number of captures that will end the game
//Flag_Preset_Name:string		Flag Preset Name
//Building_To_Destroy1=0:int	1st Building to Destroy on Max Captures
//Building_To_Destroy2=0:int	2nd Building to Destroy on Max Captures
//Building_To_Destroy3=0:int	3th Building to Destroy on Max Captures
//Building_To_Destroy4=0:int	4th Building to Destroy on Max Captures
//Building_To_Destroy5=0:int	5th Building to Destroy on Max Captures
//Play_Capture_Sounds=1:int		This setting will enable the CTF Event sounds (0=disable,1=enable)

class TDA_CTF_Zone:public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int timer_id);
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	int NumCaptured;
	bool GotFlag;
	unsigned int DecorFlag;
	unsigned int FlagId;
	unsigned int PlayerId;
	bool DecorFlagPresent;
};
