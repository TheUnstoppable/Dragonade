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
#include "survival.h"

void SUR_Spawn_Delay::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Spawn_Delay"),1);
}

void SUR_Spawn_Delay::Timer_Expired(GameObject *obj,int number)
{
	Vector3 spawnpos;
	Vector3 position;
	GameObject *res;
	ActionParamsStruct aps;
	position = Get_Vector3_Parameter("Spawn_Pos");
	spawnpos.X = Commands->Get_Position(obj).X + position.X;
	spawnpos.Y = Commands->Get_Position(obj).Y + position.Y;
	spawnpos.Z = Commands->Get_Position(obj).Z + position.Z;
	switch (number)
	{
	case 1:
		res = Commands->Create_Object(Get_Parameter("Spawn_Obj"),spawnpos);
		if (Get_Int_Parameter("Do_Path") == 1)
		{
			aps.Set_Basic(this,100,1);
			aps.MoveArrivedDistance = 100;
			aps.WaypathID = Get_Int_Parameter("Spawn_Path");
			aps.WaypathSplined = true;
			aps.AttackActive = false;
			aps.AttackCheckBlocked = false;
			Commands->Action_Goto(res,aps);
		}
		break;
	}
}

//SURVIVAL GAME TYPE
//Params: 0 = Time Delay,1 = Supply Truck Name,2 = Emitter Pos,3,4,& 5 = Emitter Presets,6,7,8,= Build to destroy

void SUR_Generator_Zone::Created(GameObject *obj)
{
	Vector3 pos;
	pos = Get_Vector3_Parameter("Emitter_Pos");
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time_Delay"),1);
	curEmitter = Commands->Get_ID(Commands->Create_Object(Get_Parameter("Emitter_Preset_Good"),pos));
}

void SUR_Generator_Zone::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos;
	pos = Get_Vector3_Parameter("Emitter_Pos");
	GameObject *tmpObj;
	switch (number)
	{
	case 1:
		energy--;
		if (energy <= 0)
		{
			for (int i=1; i <= 3; i++)
			{
				tmpObj = Commands->Find_Object(Get_Int_Parameter(5 + i));				//start getting all the Building ID's and Destorying them for game over.
//				maxHealth = Commands->Get_Max_Health(tmpObj);					//get buildings max health
				Commands->Apply_Damage(tmpObj,10000,"Explosive",0);			//Apply_Damage (only one that don't crash game)
			}
		}
		else
		{
			Commands->Apply_Damage(Commands->Find_Object(curEmitter),10000,"BlamoKiller",0);
			curEmitter = Commands->Get_ID(Commands->Create_Object(Get_Parameter(6 - energy),pos));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time_Delay"),1);
		}
		break;
	}
}

void SUR_Generator_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	Vector3 pos;
	const char *pname;
	pos = Get_Vector3_Parameter("Emitter_Pos");
	pname = Get_Parameter("Supply_Truck_Preset");
	if (!_stricmp(Commands->Get_Preset_Name(enterer),pname))
	{
		energy++;
		Commands->Apply_Damage(enterer,10000,"Explosive",0);
		Commands->Apply_Damage(Commands->Find_Object(curEmitter),10000,"BlamoKiller",0);
		curEmitter = Commands->Get_ID(Commands->Create_Object(Get_Parameter(6 - energy),pos));
	}
}

void SUR_Generator_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&energy,4,1);
	Auto_Save_Variable(&curEmitter,4,1);
}

//SURVIVAL GAME TYPE: Minigen - 0 = Delay,1-4 = Turrets to blow up (I go down,you go down)!

void SUR_MiniGen::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time_Delay"),1);
}

void SUR_MiniGen::Timer_Expired(GameObject *obj,int number)
{
	switch (number)
	{
	case 1:
		Commands->Apply_Damage(obj,10000,"Explosive",0);
		break;
	}
}

void SUR_MiniGen::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *tmpObj;
	for (int i=1; i <= 4; i++)
	{
		tmpObj = Commands->Find_Object(Get_Int_Parameter(i));				//start getting all the Building ID's and Destorying them for game over.
		Commands->Apply_Damage(tmpObj,1,"BlamoKiller",0);			//Apply_Damage (only one that don't crash game)
	}
}

//THE ULTIMATE SCRIPT! NHB!

void SUR_NHB::Created(GameObject *obj)
{
	GDIGenAttackers = 0;
	vpadAttackers = 0;
	heliAttackers = 0;
	NodGenAttackers = 0;
	jobRequests = 0;
}

void SUR_NHB::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int msg;
	int prm = 0;
	GameObject *res;
	Vector3 rpos;
	Vector3 gpos;
	rpos.X = 0;
	rpos.Y = 0;
	rpos.Z = 100;
	if ((type != 503030) && (type != 504030))
		return; // If it's not a message involving a bot,forget it

	if (type == 504030)
	{
		//If it's something that entered the zone,we need to store it for later
		for (int x = 0; x < 11;x++)
		{
			if ((!Targets[x]) || (x == 10))
			{
				Targets[x] = param;
				return;
			}
		}
	}

	if (param == 500010)
	{
		//I'm dead and was a VPad Attacker
		vpadAttackers--;
	}
	if (param == 500011)
	{
		//I'm dead and was a heli Attacker
		heliAttackers--;
	}
	if (param == 500020)
	{
		//I'm dead and was a Nod Generator Attacker
		NodGenAttackers--;
	}
	if (param == 500021)
	{
		//I'm dead and was a GDI Generator Attacker
		GDIGenAttackers--;
	}
	if (param == 303031)
	{
		//Orca requesting job (right now we just send it after the GDI generator)
		msg = 503030;
		prm = Get_Int_Parameter("GDI_Gen");
		Commands->Send_Custom_Event(obj,sender,msg,prm,0);
	}
	if (param == 302031)
	{
		//APC stuff. In here,we calculate the job requests and set the bots to be dropped
		//accordingly
		msg = 503040;
		if (jobRequests < 40)
			prm = 5;
		if (jobRequests > 40)
			prm = 6;
		if (jobRequests > 50)
			prm = 8;
		Commands->Send_Custom_Event(obj,sender,msg,prm,0);
	}
	if (param == 303030)
	{
		GameObject *gen;
		jobRequests++; //If it's a job request,inc requests and calculate the job
		//NHB is somewhat aggressive,so once the GDI/Nod have destroyed 15 bots (we say 30 because
		//bots request a job when they spawn at the begaining of the message) we start realocating
		//troops for Generator attacks

		if (jobRequests == 30)
		{
			res = Commands->Find_Object(Get_Int_Parameter("Taunt1"));
			Commands->Apply_Damage(res,1000,"Explosive",0);
		}
		if (jobRequests == 40)
		{
			res = Commands->Find_Object(Get_Int_Parameter("Taunt2"));
			Commands->Apply_Damage(res,1000,"Explosive",0);
		}
		if (jobRequests == 50)
		{
			res = Commands->Find_Object(Get_Int_Parameter("Orca_Warning"));
			Commands->Apply_Damage(res,1000,"Explosive",0);

			gen = Commands->Find_Object(Get_Int_Parameter("GDI_Gen"));
			gpos = Commands->Get_Position(gen);
			rpos.Z = 100;

			rpos.X = gpos.X - 20;
			rpos.Y = gpos.Y;
			res = Commands->Create_Object(Get_Parameter("Strike_Preset"),rpos);

			rpos.X = gpos.X + 20;
			rpos.Y = gpos.Y;
			res = Commands->Create_Object(Get_Parameter("Strike_Preset"),rpos);

			rpos.Z = 0;
			rpos.X = gpos.X - 20;
			rpos.Y = gpos.Y;
			res = Commands->Create_Object("Orca Strike Controller",rpos);

			rpos.X = gpos.X + 20;
			rpos.Y = gpos.Y;
			res = Commands->Create_Object("Orca Strike Controller",rpos);

			rpos.X = gpos.X - 20;
			rpos.Y = gpos.Y + 20;
			res = Commands->Create_Object("Orca Strike Controller",rpos);

			rpos.X = gpos.X + 20;
			rpos.Y = gpos.Y - 20;
			res = Commands->Create_Object("Orca Strike Controller",rpos);
		}
		if (jobRequests < 30)
		{
			if (vpadAttackers < 3)
			{
				msg = 503030; //Signals that this is a bot command
				prm = 500010; //Attack vehicle pad cmd
				Commands->Send_Custom_Event(obj,sender,msg,prm,0);
				vpadAttackers++;
			}
			//Make sure it's an ELSE IF not IF if you add a command,not everyone makes this mistake but I have sometimes.
			//Why it's key in this case is probally fairly obvious.
			else if (heliAttackers < 2)
			{
				msg = 503030;
				prm = 500011; //Attack heli pad cmd
				Commands->Send_Custom_Event(obj,sender,msg,prm,0);
				heliAttackers++;
			}
			//The above 2 commands use 2-3 soldiers as the bots cares more about the generators
			//Those 5 bots just try to keep the enemy from getting a vehicle early on
			else if (NodGenAttackers < 5)
			{
				msg = 503030;
				prm = 500020; //Attack Nod's gen cmd
				Commands->Send_Custom_Event(obj,sender,msg,prm,0);
				NodGenAttackers++;
			}
			else
			{
				msg = 503030;
				prm = 500021; //Attack GDI's gen cmd
				Commands->Send_Custom_Event(obj,sender,msg,prm,0);
				GDIGenAttackers++;
			}
		}
		else
		{
			//Ok,they got 15 kills against us.......time to be a little more aggressive!
			//We send more troops based on which generator has the most damage and how many bots they
			//have killed. As in: If we've "lost" 30 bots,we attack the most damaged generator
			//with every group and the 2 generator attackers.

			float GDIGenHP;
			float NodGenHP;
			int random;

			gen = Commands->Find_Object(Get_Int_Parameter("GDI_Gen"));
			GDIGenHP = Commands->Get_Health(gen);
			gen = Commands->Find_Object(Get_Int_Parameter("Nod_Gen"));
			NodGenHP = Commands->Get_Health(gen);

			if ((vpadAttackers < 3) && (jobRequests < 40))
			{
				msg = 503030; //Signals that this is a bot command
				prm = 500010; //Attack vehicle pad cmd
				Commands->Send_Custom_Event(obj,sender,msg,prm,0);
				vpadAttackers++;
			}
			else
			{
				random = Commands->Get_Random_Int(0,2);
				if (!random)
				{
					if (NodGenHP < GDIGenHP)
					{
						msg = 503030;
						prm = 500020; //Attack Nod's gen cmd
						Commands->Send_Custom_Event(obj,sender,msg,prm,0);
						NodGenAttackers++;
					}
					else
					{
						msg = 503030;
						prm = 500021; //Attack GDI's gen cmd
						Commands->Send_Custom_Event(obj,sender,msg,prm,0);
						GDIGenAttackers++;
					}
				}
				if (random == 1)
				{
					msg = 503030;
					prm = 500020; //Attack Nod's gen cmd
					Commands->Send_Custom_Event(obj,sender,msg,prm,0);
					NodGenAttackers++;
				}
				if (random == 2)
				{
					msg = 503030;
					prm = 500021; //Attack GDI's gen cmd
					Commands->Send_Custom_Event(obj,sender,msg,prm,0);
					GDIGenAttackers++;
				}
			}
		}
	}
}

void SUR_NHB::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&vpadAttackers,4,1);
	Auto_Save_Variable(&heliAttackers,4,2);
	Auto_Save_Variable(&NodGenAttackers,4,3);
	Auto_Save_Variable(&GDIGenAttackers,4,4);
	Auto_Save_Variable(&jobRequests,4,5);
}

//Soldier script so that the bots can react apporpiatly according to commands
//(slightly pointless to send commands when no one listens.......you've probally been in one of those
//situations before,so I'm sure you'd know...)

void SUR_NHB_Soldier::Created(GameObject *obj)
{
	GameObject *targ;
	int msg;
	int prm;
	msg = 503030;
	prm = 303030;
	atDes = false;
	targ = Commands->Find_Object(Get_Int_Parameter("NHB_ID"));
	Commands->Send_Custom_Event(obj,targ,msg,prm,0);
	Commands->Start_Timer(obj,this,5.000,1);
	oldPos = Commands->Get_Position(obj);
}

void SUR_NHB_Soldier::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	GameObject *targ;
	char sparam[100];
	if (type != 503030)
		return; //Like above,if it's not a bot message then forget it...
	command = param; //Need to store the command so we know the parameter when we die,so NHB
	//can calculate correctly...

	//Rest is attack stuff. Params are just like the ones above that NHB sends.
	//Bots just set their aggressiveness and head to their waypoints.
	if (param == 500010)
	{
		Commands->Set_Innate_Aggressiveness(obj,50.500);
		sprintf(sparam,"%d,%d,%d",Get_Int_Parameter("VPad_Path"),Get_Int_Parameter("VPad_Path_Start"),Get_Int_Parameter("VPad_Path_End"));
		Commands->Attach_Script(obj,"PDS_Test_Follow_Waypath",sparam);
	}
	if (param == 500011)
	{
		Commands->Set_Innate_Aggressiveness(obj,50.500);
		sprintf(sparam,"%d,%d,%d",Get_Int_Parameter("HeliPath"),Get_Int_Parameter("HeliPath_Start"),Get_Int_Parameter("HeliPath_End"));
		Commands->Attach_Script(obj,"PDS_Test_Follow_Waypath",sparam);
	}
	if (param == 500020)
	{
		Commands->Set_Innate_Aggressiveness(obj,50.500);
		sprintf(sparam,"%d,%d,%d",Get_Int_Parameter("NodGen_Path"),Get_Int_Parameter("NodGen_Path_Start"),Get_Int_Parameter("NodGen_Path_End"));
		Commands->Attach_Script(obj,"PDS_Test_Follow_Waypath",sparam);
	}
	if (param == 500021)
	{
		Commands->Set_Innate_Aggressiveness(obj,50.000);
		sprintf(sparam,"%d,%d,%d",Get_Int_Parameter("GDIGen_Path"),Get_Int_Parameter("GDIGen_Path_Start"),Get_Int_Parameter("GDIGen_Path_End"));
		Commands->Attach_Script(obj,"PDS_Test_Follow_Waypath",sparam);
	}
	if ((param == 501120) && (!atDes))
	{
		if (command == 500020)
			targ = Commands->Find_Object(Get_Int_Parameter("NodGen_ID"));
		if (command == 500021)
			targ = Commands->Find_Object(Get_Int_Parameter("GDIGen_ID"));
		Commands->Select_Weapon(obj,"CnC_Weapon_MineTimed_Player");
		atDes = true;
	}
}

void SUR_NHB_Soldier::Timer_Expired(GameObject *obj,int number)
{
	Vector3 apos;
	char sparam[100];
	
	apos = Commands->Get_Position(obj);

	if (atDes)
		return;
	if ((oldPos.X != apos.X) || (oldPos.Y != apos.Y) || (oldPos.Z != apos.Z))
	{
		oldPos = Commands->Get_Position(obj);
		Commands->Start_Timer(obj,this,10.000,1);
		return;
	}
	if (command == 500010)
	{
		Commands->Set_Innate_Aggressiveness(obj,50.500);
		sprintf(sparam,"%d,%d,%d",Get_Int_Parameter("VPad_Path"),Get_Int_Parameter("VPad_Path_Start"),Get_Int_Parameter("VPad_Path_End"));
		Commands->Attach_Script(obj,"PDS_Test_Follow_Waypath",sparam);
	}
	if (command == 500011)
	{
		Commands->Set_Innate_Aggressiveness(obj,50.500);
		sprintf(sparam,"%d,%d,%d",Get_Int_Parameter("HeliPath"),Get_Int_Parameter("HeliPath_Start"),Get_Int_Parameter("HeliPath_End"));
		Commands->Attach_Script(obj,"PDS_Test_Follow_Waypath",sparam);
	}
	if (command == 500020)
	{
		Commands->Set_Innate_Aggressiveness(obj,50.500);
		sprintf(sparam,"%d,%d,%d",Get_Int_Parameter("NodGen_Path"),Get_Int_Parameter("NodGen_Path_Start"),Get_Int_Parameter("NodGen_Path_End"));
		Commands->Attach_Script(obj,"PDS_Test_Follow_Waypath",sparam);
	}
	if (command == 500021)
	{
		Commands->Set_Innate_Aggressiveness(obj,50.000);
		sprintf(sparam,"%d,%d,%d",Get_Int_Parameter("GDIGen_Path"),Get_Int_Parameter("GDIGen_Path_Start"),Get_Int_Parameter("GDIGen_Path_End"));
		Commands->Attach_Script(obj,"PDS_Test_Follow_Waypath",sparam);
	}
//	oldPos = Commands->Get_Position(obj);
	Commands->Start_Timer(obj,this,5.000,1);
}

void SUR_NHB_Soldier::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *targ;
	int msg;
	msg = 503030;
	targ = Commands->Find_Object(Get_Int_Parameter("NHB_ID"));
	Commands->Send_Custom_Event(obj,targ,msg,command,0);
}

void SUR_NHB_Soldier::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&command,4,1);
	Auto_Save_Variable(&oldPos,4,2);
	Auto_Save_Variable(&atDes,4,3);
}

//ObjDestroyer is used for the generators. Essintally it takes down buildings or anything else
//that needs to be destroyed when this obj blows up.

void SUR_Obj_Destroyer::Death(GameObject *obj,GameObject *killer)
{
	GameObject *tmpObj;
	tmpObj = Commands->Find_Object(Get_Int_Parameter("ID"));
	Commands->Apply_Damage(tmpObj,9999,"Explosive",0);
}

//New PT is ment for vehicles.

void SUR_New_PT::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Vector3 pos;
	GameObject *res;
	int msg;
	int prm;
	const char *name;
	char sparam[100];
	float rot;
	msg = Get_Int_Parameter("Message");
	prm = 0;
	res = Commands->Find_Object(Get_Int_Parameter("Marker_ID"));
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Send_Custom_Event(obj,res,msg,prm,0);

		rot = Commands->Get_Facing(obj); //Get the terminal's rotation
		pos = Commands->Get_Position(obj); //Get the terminal's position
		name = Commands->Get_Preset_Name(obj); //Get the terminal's preset name
		sprintf(sparam,"%d,%d",Get_Int_Parameter("Marker_ID"),Get_Int_Parameter("Message")); //Get the parameter

		Commands->Apply_Damage(obj,9999,"BlamoKiller",0); //Kill the terminal

		res = Commands->Create_Object(name,pos);
		Commands->Set_Facing(res,rot);
		Commands->Attach_Script(res,"SUR_New_PT",sparam);
	}
}

void SUR_New_PT_Reciever::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Vector3 pos;
	GameObject *res;
	float rot;
	int e;
	e = Get_Int_Parameter("Message");
	if (type == e)
	{
		rot = Commands->Get_Facing(obj); //Get our rotation
		pos = Commands->Get_Position(obj); //Get our position

		res = Commands->Create_Object(Get_Parameter("Preset_Name"),pos);
		Commands->Set_Facing(res,rot);
	}
}

//APC code. Asks for how many bots to spit out,then when it gets the right msg,it spits the
//bots out!

void SUR_APC_AI::Created(GameObject *obj)
{
	GameObject *tmpObj;
	int msg;
	int prm;
	msg = 503030; //Bot msg
	prm = 302031; //APC requesting job instead of soldier
	tmpObj = Commands->Find_Object(Get_Int_Parameter("NHB_ID"));
	Commands->Send_Custom_Event(obj,tmpObj,msg,prm,0);
}

void SUR_APC_AI::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	Vector3 posM;
	Vector3 rpos;
	if (type != 503040)
		return;
	if (param == 202020)
	{
		//Drop the bots.
		posM = Commands->Get_Position(obj); //Get our position
		for (int i=1; i <= aiCount; i++)
		{
			rpos.X = posM.X + (5*i);
			rpos.Y = posM.Y + 10;
			rpos.Z = posM.Z + 2;
			Commands->Create_Object(Get_Parameter("Preset_To_Create"),rpos);
		}
		Commands->Attach_Script(obj,"JFW_Base_Defense","0,300,10");
	}
	else
	{
//		Commands->Send_Console_Message_Int(param,"Recieved Load Count: %d ");
		aiCount = param;
	}
}

void SUR_APC_AI::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&aiCount,4,1);
}

//Orca AI,just attacks the target it recieves

void SUR_Orca_AI::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type != 503030)
		return;
	
	ActionParamsStruct action;
	GameObject *targ;
	Vector3 offset;
	Vector3 targPos;

	offset = Get_Vector3_Parameter("Attack_Offset");
	action.Set_Basic(this,100,10); //Set the basic stuff
	targ = Commands->Find_Object(param); //Find the target
	targPos = Commands->Get_Position(targ);
	targPos.X+=offset.X;
	targPos.Y+=offset.Y;
	targPos.Z+=offset.Z;
	
	action.MoveArrivedDistance = 100;
	action.Set_Movement(targPos,5,30,false);
	action.Set_Attack(targ,30,0,true); //Set the attack stuff
	Commands->Action_Attack(obj,action); //Attack!
//	Commands->Send_Console_Message_Int(param,"Target Recieved: %d \n");
}

void SUR_Orca_AI::Created(GameObject *obj)
{
	GameObject *tmpObj;
	int msg;
	int prm;
	msg = 503030; //Bot msg
	prm = 303031; //Orca requesting job instead of soldier
	tmpObj = Commands->Find_Object(Get_Int_Parameter("NHB_ID"));
	Commands->Send_Custom_Event(obj,tmpObj,msg,prm,0);
}

void SUR_Orca_AI::Death(GameObject *obj,GameObject *killer)
{
	GameObject *tmpObj;
	int msg;
	int prm;
	msg = 503030; //Bot msg
	prm = 502001; //Orca death
	tmpObj = Commands->Find_Object(Get_Int_Parameter("NHB_ID"));
	Commands->Send_Custom_Event(obj,tmpObj,msg,prm,0);
}

void SUR_Orca_AI::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	if (action_id != 20)
		return;

	GameObject *tmpObj;
	int msg;
	int prm;
	msg = 503030; //Bot msg
	prm = 303031; //Orca requesting job instead of soldier
	tmpObj = Commands->Find_Object(Get_Int_Parameter("NHB_ID"));
	Commands->Send_Custom_Event(obj,tmpObj,msg,prm,0);
}

//Chinook AI
//Drops off a giving vehicle when it's sent a certain custom

void SUR_Chinook_AI::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type != 503030)
		return;
	
	Vector3 offset;
	Vector3 targPos;
	GameObject *res;
	float rot;

	offset = Get_Vector3_Parameter("Position_Offset");
	targPos = Commands->Get_Position(obj);
	targPos.X+=offset.X;
	targPos.Y+=offset.Y;
	targPos.Z+=offset.Z;
	rot = Commands->Get_Facing(obj);
	res = Commands->Create_Object(Get_Parameter("Preset_Name"),targPos);
	Commands->Set_Facing(res,rot);
}

//Deployable turret script. Only shoots what's not on GDI or Nod.

void SUR_Dep_Turret::Created(GameObject *obj)
{
	Vector3 pos,pos1,pos2,pos3;
	GameObject *object;
	objtype = Commands->Get_Player_Type(obj);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	pos = Commands->Get_Position(obj);
	pos1.X = pos.X - 10;
	pos1.Y = pos.Y - 10;
	pos1.Z = pos.Z + 2;
	pos2.X = pos.X + 10;
	pos2.Y = pos.Y;
	pos2.Z = pos.Z + 2;
	pos3.X = pos.X + 10;
	pos3.Y = pos.Y - 10;
	pos3.Z = pos.Z + 2;
	object = Commands->Create_Object("Invisible_Object",pos1);
	if (object)
		id1 = Commands->Get_ID(object);
	object = Commands->Create_Object("Invisible_Object",pos2);
	if (object)
		id2 = Commands->Get_ID(object);
	object = Commands->Create_Object("Invisible_Object",pos3);
	if (object)
		id3 = Commands->Get_ID(object);
	Commands->Start_Timer(obj,this,10,1);
}

void SUR_Dep_Turret::EnemySeen(GameObject *obj,GameObject *enemy)
{
	ActionParamsStruct params;
	Vector3 pos,pos2;
	float distance;
	float minattack,maxattack;
	float attacktimer;
	pos = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(enemy);
	distance = Commands->Get_Distance(pos2,pos);
	minattack = Get_Float_Parameter("MinAttackDistance");
	if ((distance >= minattack) && (CheckPlayerType(enemy,0) > 0) && (CheckPlayerType(enemy,1) > 0))
	{
		maxattack = Get_Float_Parameter("MaxAttackDistance");
		params.Set_Basic(this,100,2);
		params.Set_Attack(enemy,maxattack,0.0,true);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
		attacktimer = Get_Float_Parameter("AttackTimer");
		Commands->Start_Timer(obj,this,attacktimer,2);
	}
}

void SUR_Dep_Turret::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	if (action_id == 2)
	{
		Commands->Action_Reset(obj,100);
	}
}

void SUR_Dep_Turret::Timer_Expired(GameObject *obj,int number)
{
	ActionParamsStruct var;
	GameObject *object;
	float f;
	switch (number)
	{
	case 1:
		f = Commands->Get_Random(0.0f,2.9999f);
		switch (__min(int(f),2))
		{
		case 0:
			object = Commands->Find_Object(id1);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0,0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		case 1:
			object = Commands->Find_Object(id2);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0,0,true);
				Commands->Action_Attack(obj,var);
			}
			break;
		default:
			object = Commands->Find_Object(id3);
			if (object)
			{
				var.Set_Basic(this,70,1);
				var.Set_Attack(object,0,0,true);
				Commands->Action_Attack(obj,var);
			}
		}
	Commands->Start_Timer(obj,this,10,1);
	break;
	case 2:
		Commands->Action_Reset(obj,100);
		break;
	}
}

void SUR_Dep_Turret::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,4,1);
	Auto_Save_Variable(&id2,4,2);
	Auto_Save_Variable(&id3,4,3);
	Auto_Save_Variable(&objtype,4,4);
}

void SUR_Timed_Death::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}

void SUR_Timed_Death::Timer_Expired(GameObject *obj,int number)
{
	Commands->Apply_Damage(obj,Get_Float_Parameter("Damage_Amount"),Get_Parameter("Damage_Type"),0);
}

void SUR_GrantPowerup_OnEntry::Entered(GameObject *obj,GameObject *enterer)
{
	if ((!_stricmp(Commands->Get_Preset_Name(enterer),Get_Parameter("Preset"))) || (!_stricmp(Get_Parameter("Preset"),"Any")))
	{
		Commands->Give_PowerUp(enterer,Get_Parameter("Powerup"),true);
	}
}

ScriptRegistrant<SUR_Spawn_Delay> SUR_Spawn_Delay_Registrant("SUR_Spawn_Delay","Spawn_Delay:float,Spawn_Pos:vector3,Spawn_Obj:string,Spawn_Path:int,Do_Path:int");
ScriptRegistrant<SUR_Generator_Zone> SUR_Generator_Zone_Registrant("SUR_Generator_Zone","Time_Delay:float,Supply_Truck_Preset:string,Emitter_Pos:vector3,Emitter_Preset_Good:string,Emitter_Preset_Ok:string,Emitter_Preset_Bad:string,Build_D_One:int,Build_D_Two:int,Build_D_Three:int");
ScriptRegistrant<SUR_MiniGen> SUR_MiniGen_Registrant("SUR_MiniGen","Time_Delay:float,Turret_ID_One:int,Turret_ID_Two:int,Turret_ID_Three:int,Turret_ID_Four:int");
ScriptRegistrant<SUR_NHB> SUR_NHB_Registrant("SUR_NHB","GDI_Gen:int,Nod_Gen:int,Strike_Preset:string,Taunt1:int,Taunt2:int,OrcaWarning:int");
ScriptRegistrant<SUR_NHB_Soldier> SUR_NHB_Soldier_Registrant("SUR_NHB_Soldier","NHB_ID:int,VPad_Path:int,VPad_Path_Start:int,VPad_Path_End:int,HeliPath:int,HeliPath_Start:int,HeliPath_End:int,NodGen_Path:int,NodGen_Path_Start:int,NodGen_Path_End:int,GDIGen_Path:int,GDIGen_Path_Start:int,GDIGen_Path_End:int,NodGen_ID:int,GDIGen_ID:int");
ScriptRegistrant<SUR_Obj_Destroyer> SUR_Obj_Destroyer_Registrant("SUR_Obj_Destroyer","ID:int");
ScriptRegistrant<SUR_New_PT> SUR_New_PT_Registrant("SUR_New_PT","Marker_ID:int,Message:int");
ScriptRegistrant<SUR_New_PT_Reciever> SUR_New_PT_Reciever_Registrant("SUR_New_PT_Reciever","Preset_Name:string,Message:int");
ScriptRegistrant<SUR_APC_AI> SUR_APC_AI_Registrant("SUR_APC_AI","Preset_To_Create:string,NHB_ID:int");
ScriptRegistrant<SUR_Orca_AI> SUR_Orca_AI_Registrant("SUR_Orca_AI","NHB_ID:int,Attack_Offset:vector3");
ScriptRegistrant<SUR_Chinook_AI> SUR_Chinook_AI_Registrant("SUR_Chinook_AI","Preset_Name:string,Position_Offset:vector3");
ScriptRegistrant<SUR_Dep_Turret> SUR_Dep_Turret_Registrant("SUR_Dep_Turret","MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:int");
ScriptRegistrant<SUR_Timed_Death> SUR_Timed_Death_Registrant("SUR_Timed_Death","Delay:float,Damage_Amount:float,Damage_Type:string");
ScriptRegistrant<SUR_GrantPowerup_OnEntry> SUR_GrantPowerup_OnEntry_Registrant("SUR_GrantPowerup_OnEntry","Preset:string,Powerup:string");
