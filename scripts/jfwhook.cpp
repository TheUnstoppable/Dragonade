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
#include "general.h"

#include "scripts.h"
#include "engine.h"
#include "jfwhook.h"
#include "PhysicalGameObj.h"

void ObjectCreateHookBaseHook(void *data,GameObject *obj)
{
	JFW_Object_Created_Hook_Base *base = (JFW_Object_Created_Hook_Base *)data;
	base->ObjectCreateHook(obj);
}

JFW_Object_Created_Hook_Base::~JFW_Object_Created_Hook_Base()
{
	RemoveHook();
}

void JFW_Object_Created_Hook_Base::Detach(GameObject *obj)
{
	ScriptImpClass::Detach(obj);
	RemoveHook();
}

void JFW_Object_Created_Hook_Base::Created(GameObject *obj)
{
	hookid = 0;
	InstallHook();
}

void JFW_Object_Created_Hook_Base::InstallHook()
{
	if (AddObjectCreateHook)
	{
		h = new ObjectCreateHookStruct;
		h->data = (void *)this;
		h->hook = ObjectCreateHookBaseHook;
		RemoveHook();
		hookid = AddObjectCreateHook(h);
	}
}

void JFW_Object_Created_Hook_Base::RemoveHook()
{
	if (RemoveObjectCreateHook && hookid)
	{
		RemoveObjectCreateHook(hookid);
		hookid = 0;
		if (h)
		{
			delete h;
			h = 0;
		}
	}
}

void JFW_Attach_Script_Preset_Created::ObjectCreateHook(GameObject *obj)
{
	const char *script;
	const char *paramx;
	char *params;
	char delim;
	script = Get_Parameter("Script");
	paramx = Get_Parameter("Params");
	params = newstr(paramx);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Attach_Script_Is_Preset(obj,Get_Parameter("Preset"),script,params,Get_Int_Parameter("Player_Type"));
	delete[] params;
}

void JFW_Attach_Script_Type_Created::ObjectCreateHook(GameObject *obj)
{
	const char *script;
	const char *paramx;
	char *params;
	char delim;
	script = Get_Parameter("Script");
	paramx = Get_Parameter("Params");
	params = newstr(paramx);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Attach_Script_Is_Type(obj,Get_Int_Parameter("Type"),script,params,Get_Int_Parameter("Player_Type"));
	delete[] params;
}

void JFW_Attach_Script_Player_Created::Created(GameObject *obj)
{
	hookid = 0;
	InstallHook();
	const char *script;
	const char *paramx;
	char *params;
	char delim;
	script = Get_Parameter("Script");
	paramx = Get_Parameter("Params");
	params = newstr(paramx);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Attach_Script_Player_Once(script,params,Get_Int_Parameter("Player_Type"));
	delete[] params;
}

void JFW_Attach_Script_Player_Created::ObjectCreateHook(GameObject *obj)
{
	const char *script;
	const char *paramx;
	char *params;
	char delim;
	script = Get_Parameter("Script");
	paramx = Get_Parameter("Params");
	params = newstr(paramx);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	if (Commands->Is_A_Star(obj))
	{
		Attach_Script_Is_Type(obj,CID_Soldier,script,params,Get_Int_Parameter("Player_Type"));
	}
	delete[] params;
}

void JFW_Attach_Script_Vehicle_Created::ObjectCreateHook(GameObject *obj)
{
	const char *script;
	const char *paramx;
	char *params;
	char delim;
	script = Get_Parameter("Script");
	paramx = Get_Parameter("Params");
	params = newstr(paramx);
	delim = Get_Parameter("Delim")[0];
	unsigned int x = strlen(params);
	for (unsigned int i=0;i<x;i++)
	{
		if (params[i] == delim)
		{
			params[i] = ',';
		}
	}
	Attach_Script_Is_Type(obj,CID_Vehicle,script,params,Get_Int_Parameter("Player_Type"));
	delete[] params;
}

void KeyHookBaseHook(void *data)
{
	JFW_Key_Hook_Base *base = (JFW_Key_Hook_Base *)data;
	base->KeyHook();
}

void JFW_Key_Hook_Base::InstallHook(const char *keyname,GameObject *obj)
{
	if (!hookid)
	{
		k = new KeyHookStruct;
		k->data = this;
		k->hook = KeyHookBaseHook;
		k->PlayerID = Get_Player_ID(obj);
		k->key = newstr(keyname);
		hookid = AddKeyHook(k);
	}
}

void JFW_Key_Hook_Base::RemoveHook()
{
	if (RemoveKeyHook && hookid)
	{
		RemoveKeyHook(hookid);
		hookid = 0;
		if (k)
		{
			if (k->key)
			{
				delete[] k->key;
			}
			delete k;
			k = 0;
		}
	}
}

void JFW_Key_Hook_Base::Destroyed(GameObject *obj)
{
	RemoveHook();
}

void JFW_Key_Hook_Base::Detach(GameObject *obj)
{
	ScriptImpClass::Detach(obj);
	RemoveHook();
}

JFW_Key_Hook_Base::~JFW_Key_Hook_Base()
{
	RemoveHook();
}

// -------------------------------------------------------------------------------------------------
// Multi key hook script
// -------------------------------------------------------------------------------------------------

void MultiKeyHookScriptImpClass::Detach(GameObject *obj)
{
  if(Exe != EXE_LEVELEDIT)
  {
    Destroyed(obj);   // Double check to make sure we're all cleaned up
  }
  ScriptImpClass::Detach(obj);
}

// -------------------------------------------------------------------------------------------------

void MultiKeyHookScriptImpClass::Destroyed(GameObject *obj)
{
  SLNode<KeyHookWrapperStruct>* head = ms_keyhooks.Head();
  while(NULL != head)
  {
    RemoveHook(head->Data()->hook.key, obj);
    head = ms_keyhooks.Head();
  }
}

// -------------------------------------------------------------------------------------------------

void MultiKeyHookBaseHook(void *data)
{
  MultiKeyHookScriptImpClass::KeyHookWrapperStruct* hook = (MultiKeyHookScriptImpClass::KeyHookWrapperStruct*)data;
  hook->owner->KeyHook(hook->hook.key);
}

// -------------------------------------------------------------------------------------------------

void MultiKeyHookScriptImpClass::InstallHook(const char* logicalKey, GameObject *obj)
{
  KeyHookWrapperStruct* hookWrapper = FindHook(logicalKey);
  if (NULL == hookWrapper)
  {
    hookWrapper = new KeyHookWrapperStruct();
    hookWrapper->owner = this;
    hookWrapper->hook.data = hookWrapper;
    hookWrapper->hook.hook = MultiKeyHookBaseHook;
    hookWrapper->hook.PlayerID = Get_Player_ID(obj);
    hookWrapper->hook.key = newstr(logicalKey);
    hookWrapper->hookId = AddKeyHook(&hookWrapper->hook);
    ms_keyhooks.Add_Tail(hookWrapper);
  }
}

// -------------------------------------------------------------------------------------------------

void MultiKeyHookScriptImpClass::RemoveHook(const char* logicalKey, GameObject *obj)
{
  KeyHookWrapperStruct* hookWrapper = FindHook(logicalKey);
  if (NULL != hookWrapper)
  {
    RemoveKeyHook(hookWrapper->hookId);
    ms_keyhooks.Remove(hookWrapper);

    if (NULL != hookWrapper->hook.key)
      delete [] hookWrapper->hook.key;
    delete hookWrapper;
  }
}

// -------------------------------------------------------------------------------------------------

MultiKeyHookScriptImpClass::KeyHookWrapperStruct* MultiKeyHookScriptImpClass::FindHook(const char* logicalKey)
{
  for (SLNode<KeyHookWrapperStruct>* node = ms_keyhooks.Head(); node; node = node->Next())
  {
    if (0 == strcmp(logicalKey, node->Data()->hook.key))
      return node->Data();
  }

  return NULL;
}

// -------------------------------------------------------------------------------------------------




void JFW_Jetpack::Detach(GameObject *obj)
{
	if (Exe != 4)
	{
		if (Get_Fly_Mode(obj))
		{
			Toggle_Fly_Mode(obj);
		}
	}
	JFW_Key_Hook_Base::Detach(obj);
}

void JFW_Jetpack::Created(GameObject *obj)
{
	enabled = true;
	hookid = 0;
	k = 0;
	time = true;
	InstallHook("Jetpack",obj);
}

void JFW_Jetpack::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enabled = true;
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enabled = false;
	}
}

void JFW_Jetpack::KeyHook()
{
	if (enabled && !Get_Vehicle(Owner()))
	{
		if (time)
		{
			if(!Get_Fly_Mode(Owner()))
			{
				Commands->Set_Animation(Owner(), Get_Parameter("OnAnimation"), true, 0, 0, -1, true);
			}
			else
			{
				Owner()->As_PhysicalGameObj()->Clear_Animation();
			}
			Toggle_Fly_Mode(Owner());
			time = false;
			Commands->Start_Timer(Owner(),this,0.1f,1);
		}
	}
}

void JFW_Jetpack::Timer_Expired(GameObject *obj,int number)
{
	time = true;
}

void JFW_Jetpack_Timer::Detach(GameObject *obj)
{
	if (Exe != 4)
	{
		if (Get_Fly_Mode(obj))
		{
			Toggle_Fly_Mode(obj);
		}
	}
	JFW_Key_Hook_Base::Detach(obj);
}

void JFW_Jetpack_Timer::Created(GameObject *obj)
{
	enabled = true;
	hookid = 0;
	k = 0;
	time = true;
	InstallHook("Jetpack",obj);
}

void JFW_Jetpack_Timer::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enabled = true;
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enabled = false;
	}
}

void JFW_Jetpack_Timer::KeyHook()
{
	if (enabled && !Get_Vehicle(Owner()))
	{
		if (time)
		{
			if(!Get_Fly_Mode(Owner()))
			{
				Commands->Set_Animation(Owner(), Get_Parameter("OnAnimation"), true, 0, 0, -1, true);
			}
			else
			{
				Owner()->As_PhysicalGameObj()->Clear_Animation();
			}
			Toggle_Fly_Mode(Owner());
			time = false;
			Commands->Start_Timer(Owner(),this,Get_Float_Parameter("Timer"),1);
		}
		else
		{
			unsigned int red;
			unsigned int green;
			unsigned int blue;
			Get_Team_Color(Get_Player_Type(Owner()),&red,&green,&blue);
			Send_Message_Player(Owner(),red,green,blue,Get_Parameter("Message"));
		}
	}
}

void JFW_Jetpack_Timer::Timer_Expired(GameObject *obj,int number)
{
	time = true;
}

void JFW_Limited_Jetpack_Timer::Detach(GameObject *obj)
{
	if (Exe != 4)
	{
		if (Get_Fly_Mode(obj))
		{
			Toggle_Fly_Mode(obj);
		}
	}
	JFW_Key_Hook_Base::Detach(obj);
}

void JFW_Limited_Jetpack_Timer::Created(GameObject *obj)
{
	enabled = true;
	hookid = 0;
	k = 0;
	timeon = true;
	timeoff = true;
	InstallHook("Jetpack",obj);
}

void JFW_Limited_Jetpack_Timer::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enabled = true;
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enabled = false;
	}
}

void JFW_Limited_Jetpack_Timer::KeyHook()
{
	if (enabled && !Get_Vehicle(Owner()))
	{
		if ((!Get_Fly_Mode(Owner()) && timeon) || (Get_Fly_Mode(Owner()) && timeoff))
		{
			if(!Get_Fly_Mode(Owner()))
			{
				Commands->Set_Animation(Owner(), Get_Parameter("OnAnimation"), true, 0, 0, -1, true);
				timeoff = false;
				Commands->Start_Timer(Owner(),this,Get_Float_Parameter("TimerOff"),1);
				Commands->Start_Timer(Owner(),this,Get_Float_Parameter("TimerUse"),3);
			}
			else
			{
				Owner()->As_PhysicalGameObj()->Clear_Animation();
				timeon = false;
				Commands->Start_Timer(Owner(),this,Get_Float_Parameter("TimerOn"),2);
			}
			Toggle_Fly_Mode(Owner());
		}
		else
		{
			unsigned int red;
			unsigned int green;
			unsigned int blue;
			Get_Team_Color(Get_Player_Type(Owner()),&red,&green,&blue);
			Send_Message_Player(Owner(),red,green,blue,Get_Parameter("Message"));
		}
	}
}

void JFW_Limited_Jetpack_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		timeoff = true;
	}
	else if (number == 2)
	{
		timeon = true;
	}
	else if ((number == 3) && Get_Fly_Mode(Owner()))
	{
		KeyHook();
	}
}

void JFW_Jetpack_Model::Detach(GameObject *obj)
{
	if (Exe != 4)
	{
		if (Get_Fly_Mode(obj))
		{
			Toggle_Fly_Mode(obj);
		}
	}
	JFW_Key_Hook_Base::Detach(obj);
}

void JFW_Jetpack_Model::Created(GameObject *obj)
{
	enabled = true;
	hookid = 0;
	k = 0;
	time = true;
	InstallHook("Jetpack",obj);
}

void JFW_Jetpack_Model::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enabled = true;
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enabled = false;
	}
}

void JFW_Jetpack_Model::KeyHook()
{
	if (enabled && !Get_Vehicle(Owner()))
	{
		if (time)
		{
			if (!Get_Fly_Mode(Owner()))
			{
				Commands->Set_Model(Owner(),Get_Parameter("OnModel"));
				Commands->Set_Animation(Owner(), Get_Parameter("OnAnimation"), true, 0, 0, -1, true);
			}
			else
			{
				Commands->Set_Model(Owner(),Get_Parameter("OffModel"));
				Owner()->As_PhysicalGameObj()->Clear_Animation();
			}
			Toggle_Fly_Mode(Owner());
			time = false;
			Commands->Start_Timer(Owner(),this,0.1f,1);
		}
	}
}

void JFW_Jetpack_Model::Timer_Expired(GameObject *obj,int number)
{
	time = true;
}

void JFW_Dplbl_Vhcls_Keyboard::Created(GameObject *obj)
{
	pilotID = 0;
	CanDestroyAnim = true;
	hookid = 0;
	k = 0;
}

void JFW_Dplbl_Vhcls_Keyboard::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!pilotID)
		{
			InstallHook("Deploy",sender);
			pilotID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (pilotID == Commands->Get_ID(sender))
		{
			RemoveHook();
			pilotID = 0;
		}
	}
}

void JFW_Dplbl_Vhcls_Keyboard::KeyHook()
{
	Force_Occupants_Exit(Owner());
	GameObject *Animation;
	Vector3 position;
	int TempCalcul;
	position.X = 0;
	position.Y = 0;
	position.Z = 0;
	Animation = Commands->Create_Object(Get_Parameter("Animation_Preset"),position);
	TempCalcul = (int)(Commands->Get_Max_Health(Animation)*(Commands->Get_Health(Owner())/Commands->Get_Max_Health(Owner())));
	Commands->Set_Health(Animation,(float)TempCalcul);
	TempCalcul = (int)(Commands->Get_Max_Shield_Strength(Animation)*(Commands->Get_Shield_Strength(Owner())/Commands->Get_Max_Shield_Strength(Owner())));
	Commands->Set_Shield_Strength(Animation,(float)TempCalcul);
	Commands->Attach_To_Object_Bone(Animation,Owner(),"origin");
	CanDestroyAnim = false;
	Commands->Apply_Damage(Owner(),Get_Float_Parameter("oldTnk_Dammage"),Get_Parameter("oldTnk_Warhead"),0);
}

void JFW_Dplbl_Vhcls_Keyboard::Killed(GameObject *obj,GameObject *killer)
{
	Vector3 position;
	if (CanDestroyAnim)
	{
		position = Commands->Get_Bone_Position(obj,"origin");
		Commands->Create_Explosion(Get_Parameter("Explosion_preset"),position,0);
	}
}

void JFW_Dplbl_Vhcls_Keyboard::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pilotID,4,1);
	Auto_Save_Variable(&CanDestroyAnim,1,2);
}

void JFW_Underground_Logic::Created(GameObject *obj)
{
	underground = false;
	enable = true;
	hookid = 0;
	k = 0;
	pilotID = 0;
}

void JFW_Underground_Logic::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!pilotID)
		{
			InstallHook("Dig",sender);
			pilotID = Commands->Get_ID(sender);
		}
	}
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (pilotID == Commands->Get_ID(sender))
		{
			RemoveHook();
			pilotID = 0;
			if (underground)
			{
				Commands->Apply_Damage(obj,100,"Death",0);
			}
		}
		if (underground)
		{
			Set_Screen_Fade_Color_Player(sender,0,0,0,0);
			Set_Screen_Fade_Opacity_Player(sender,0,0);
			Commands->Apply_Damage(sender,100,"Death",0);
		}
	}
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enable = true;
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enable = false;
	}
	if (type == Get_Int_Parameter("IndicatorMessage"))
	{
		if (underground)
		{
			Vector3 v;
			v = Commands->Get_Position(obj);
			v.Z += Get_Float_Parameter("IndicatorZOffset");
			Commands->Create_Object(Get_Parameter("IndicatorObject"),v);
		}
	}
}

void JFW_Underground_Logic::KeyHook()
{
	if (enable)
	{
		if (underground)
		{
			underground = false;
			Set_Occupants_Fade(Owner(),0,0,0,0);
			Vector3 v = Commands->Get_Position(Owner());
			v.Z -= Get_Float_Parameter("UpZOffset");
			Commands->Create_Sound(Get_Parameter("DigSound"),v,Owner());
			Commands->Create_Object(Get_Parameter("SurfaceEffectObj"),v);
			Commands->Set_Position(Owner(),v);
		}
		else
		{
			underground = true;
			Set_Occupants_Fade(Owner(),Get_Float_Parameter("DigRed"),Get_Float_Parameter("DigBlue"),Get_Float_Parameter("DigGreen"),Get_Float_Parameter("DigOpacity"));
			Vector3 v = Commands->Get_Position(Owner());
			Commands->Create_Sound(Get_Parameter("DigSound"),v,Owner());
			Commands->Create_Object(Get_Parameter("DigEffectObj"),v);
			v.Z += Get_Float_Parameter("DownZOffset");
			Commands->Set_Position(Owner(),v);
		}
	}
}

void JFW_Underground_Logic::Killed(GameObject *obj,GameObject *killer)
{
	if (underground)
	{
		Set_Occupants_Fade(obj,0,0,0,0);
		Kill_Occupants(obj);
	}
}

void JFW_Underground_Logic::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&underground,1,1);
	Auto_Save_Variable(&enable,1,2);
	Auto_Save_Variable(&pilotID,4,3);
}

void JFW_Suicide_Bomber::Created(GameObject *obj)
{
	enabled = true;
	hookid = 0;
	k = 0;
	InstallHook("BlowUp",obj);
}

void JFW_Suicide_Bomber::KeyHook()
{
	if (enabled)
	{
		Vector3 v = Commands->Get_Position(Owner());
		Commands->Create_Explosion(Get_Parameter("Explosion"),v,Owner());
		Commands->Apply_Damage(Owner(),99999,"Death",0);
	}
}

void JFW_Suicide_Bomber::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&enabled,1,1);
}

void JFW_Jetpack_Timer_No_Fly::Detach(GameObject *obj)
{
	if (Exe != 4)
	{
		if (Get_Fly_Mode(obj))
		{
			Toggle_Fly_Mode(obj);
		}
	}
	JFW_Key_Hook_Base::Detach(obj);
}

void JFW_Jetpack_Timer_No_Fly::Created(GameObject *obj)
{
	enabled = true;
	hookid = 0;
	k = 0;
	time = true;
	nofly = false;
	InstallHook("Jetpack",obj);
}

void JFW_Jetpack_Timer_No_Fly::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enabled = true;
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enabled = false;
	}
	if (type == Get_Int_Parameter("NoFlyOn"))
	{
		nofly = true;
		if (Get_Fly_Mode(obj))
		{
			obj->As_PhysicalGameObj()->Clear_Animation();
			Toggle_Fly_Mode(obj);
			time = false;
			Stop_Timer(Owner(),1);
			Commands->Start_Timer(Owner(),this,Get_Float_Parameter("Timer"),1);
		}
	}
	if (type == Get_Int_Parameter("NoFlyOff"))
	{
		nofly = false;
	}
}

void JFW_Jetpack_Timer_No_Fly::KeyHook()
{
	if (!nofly)
	{
		if (enabled && !Get_Vehicle(Owner()))
		{
			if (time)
			{
				if(!Get_Fly_Mode(Owner()))
				{
					Commands->Set_Animation(Owner(), Get_Parameter("OnAnimation"), true, 0, 0, -1, true);
				}
				else
				{
					Owner()->As_PhysicalGameObj()->Clear_Animation();
				}
				Toggle_Fly_Mode(Owner());
				time = false;
				Commands->Start_Timer(Owner(),this,Get_Float_Parameter("Timer"),1);
			}
			else
			{
				unsigned int red;
				unsigned int green;
				unsigned int blue;
				Get_Team_Color(Get_Player_Type(Owner()),&red,&green,&blue);
				Send_Message_Player(Owner(),red,green,blue,Get_Parameter("Message"));
			}
		}
	}
	else
	{
		unsigned int red;
		unsigned int green;
		unsigned int blue;
		Get_Team_Color(Get_Player_Type(Owner()),&red,&green,&blue);
		Send_Message_Player(Owner(),red,green,blue,Get_Parameter("NoFlyMessage"));
	}
}

void JFW_Jetpack_Timer_No_Fly::Timer_Expired(GameObject *obj,int number)
{
	time = true;
}

void JFW_Limited_Jetpack_Timer_No_Fly::Detach(GameObject *obj)
{
	if (Exe != 4)
	{
		if (Get_Fly_Mode(obj))
		{
			Toggle_Fly_Mode(obj);
		}
	}
	JFW_Key_Hook_Base::Detach(obj);
}

void JFW_Limited_Jetpack_Timer_No_Fly::Created(GameObject *obj)
{
	enabled = true;
	hookid = 0;
	k = 0;
	timeon = true;
	timeoff = true;
	nofly = false;
	InstallHook("Jetpack",obj);
}

void JFW_Limited_Jetpack_Timer_No_Fly::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("EnableMessage"))
	{
		enabled = true;
	}
	if (type == Get_Int_Parameter("DisableMessage"))
	{
		enabled = false;
	}
	if (type == Get_Int_Parameter("NoFlyOn"))
	{
		nofly = true;
		if (Get_Fly_Mode(obj))
		{
			obj->As_PhysicalGameObj()->Clear_Animation();
			Toggle_Fly_Mode(obj);
			timeoff = true;
			Stop_Timer(obj,1);
			Stop_Timer(obj,3);
			timeon = false;
			Commands->Start_Timer(Owner(),this,Get_Float_Parameter("TimerOn"),2);
		}
	}
	if (type == Get_Int_Parameter("NoFlyOff"))
	{
		nofly = false;
	}
}

void JFW_Limited_Jetpack_Timer_No_Fly::KeyHook()
{
	if (!nofly)
	{
		if (enabled && !Get_Vehicle(Owner()))
		{
			if ((!Get_Fly_Mode(Owner()) && timeon) || (Get_Fly_Mode(Owner()) && timeoff))
			{
				if(!Get_Fly_Mode(Owner()))
				{
					Commands->Set_Animation(Owner(), Get_Parameter("OnAnimation"), true, 0, 0, -1, true);
					timeoff = false;
					Commands->Start_Timer(Owner(),this,Get_Float_Parameter("TimerOff"),1);
					Commands->Start_Timer(Owner(),this,Get_Float_Parameter("TimerUse"),3);
				}
				else
				{
					Owner()->As_PhysicalGameObj()->Clear_Animation();
					timeon = false;
					Commands->Start_Timer(Owner(),this,Get_Float_Parameter("TimerOn"),2);
				}
				Toggle_Fly_Mode(Owner());
			}
			else
			{
				unsigned int red;
				unsigned int green;
				unsigned int blue;
				Get_Team_Color(Get_Player_Type(Owner()),&red,&green,&blue);
				Send_Message_Player(Owner(),red,green,blue,Get_Parameter("Message"));
			}
		}
	}
	else
	{
		unsigned int red;
		unsigned int green;
		unsigned int blue;
		Get_Team_Color(Get_Player_Type(Owner()),&red,&green,&blue);
		Send_Message_Player(Owner(),red,green,blue,Get_Parameter("NoFlyMessage"));
	}
}

void JFW_Limited_Jetpack_Timer_No_Fly::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		timeoff = true;
	}
	else if (number == 2)
	{
		timeon = true;
	}
	else if ((number == 3) && Get_Fly_Mode(Owner()))
	{
		KeyHook();
	}
}

ScriptRegistrant<JFW_Attach_Script_Preset_Created> JFW_Attach_Script_Preset_Created_Registrant("JFW_Attach_Script_Preset_Created","Script:string,Params:string,Delim:string,Preset:string,Player_Type:int");
ScriptRegistrant<JFW_Attach_Script_Type_Created> JFW_Attach_Script_Type_Created_Registrant("JFW_Attach_Script_Type_Created","Script:string,Params:string,Delim:string,Type:int,Player_Type:int");
ScriptRegistrant<JFW_Attach_Script_Player_Created> JFW_Attach_Script_Player_Created_Registrant("JFW_Attach_Script_Player_Created","Script:string,Params:string,Delim:string,Player_Type:int");
ScriptRegistrant<JFW_Attach_Script_Vehicle_Created> JFW_Attach_Script_Vehicle_Created_Registrant("JFW_Attach_Script_Vehicle_Created","Script:string,Params:string,Delim:string,Player_Type:int");
ScriptRegistrant<JFW_Dplbl_Vhcls_Keyboard> JFW_Dplbl_Vhcls_Keyboard_Registrant("JFW_Dplbl_Vhcls_Keyboard","Animation_Preset:string,oldTnk_Warhead:string,oldTnk_Dammage:float,Explosion_preset:string");
ScriptRegistrant<JFW_Underground_Logic> JFW_Underground_Logic_Registrant("JFW_Underground_Logic","UpZOffset:float,DownZOffset:float,DigEffectObj:string,SurfaceEffectObj:string,DisableMessage:int,EnableMessage:int,IndicatorMessage:int,IndicatorObject:string,IndicatorZOffset:float,DigRed:float,DigGreen:float,DigBlue:float,DigOpacity:float,DigSound:string");
ScriptRegistrant<JFW_Jetpack> JFW_Jetpack_Registrant("JFW_Jetpack","DisableMessage:int,EnableMessage:int,OnAnimation=null.null:string");
ScriptRegistrant<JFW_Jetpack_Timer> JFW_Jetpack_Timer_Registrant("JFW_Jetpack_Timer","DisableMessage:int,EnableMessage:int,OnAnimation=null.null:string,timer:float,Message:string");
ScriptRegistrant<JFW_Limited_Jetpack_Timer> JFW_Limited_Jetpack_Timer_Registrant("JFW_Limited_Jetpack_Timer","DisableMessage:int,EnableMessage:int,OnAnimation=null.null:string,TimerOff:float,TimerOn:Float,TimerUse:float,Message:string");
ScriptRegistrant<JFW_Jetpack_Model> JFW_Jetpack_Model_Registrant("JFW_Jetpack_Model","DisableMessage:int,EnableMessage:int,OnModel:string,OffModel:string,OnAnimation=null.null:string");
ScriptRegistrant<JFW_Suicide_Bomber> JFW_Suicide_Bomber_Registrant("JFW_Suicide_Bomber","Explosion:string");
ScriptRegistrant<JFW_Sidebar_Key_2> JFW_Sidebar_Key_2_Registrant("JFW_Sidebar_Key_2","Key=Sidebar:string,Enable_Custom=0:int,Disable_Custom=0:int,Sound:string");
ScriptRegistrant<JFW_Jetpack_Timer_No_Fly> JFW_Jetpack_Timer_No_Fly_Registrant("JFW_Jetpack_Timer_No_Fly","DisableMessage:int,EnableMessage:int,OnAnimation=null.null:string,timer:float,Message:string,NoFlyOn:int,NoFlyOff:int,NoFlyMessage:string");
ScriptRegistrant<JFW_Limited_Jetpack_Timer_No_Fly> JFW_Limited_Jetpack_Timer_No_Fly_Registrant("JFW_Limited_Jetpack_Timer_No_Fly","DisableMessage:int,EnableMessage:int,OnAnimation=null.null:string,TimerOff:float,TimerOn:Float,TimerUse:float,Message:string,NoFlyOn:int,NoFlyOff:int,NoFlyMessage:string");
