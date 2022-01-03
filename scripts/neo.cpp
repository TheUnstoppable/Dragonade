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
#include "neo.h"
#include "VehicleGameObjDef.h"
#include "SoldierGameObj.h"

//**************Script Functions*****************

//*********Neo_CreateObject_RandomTimer**********

void Neo_CreateObject_RandomTimer::Created(GameObject *obj)
{
	//Start Counting
	float timer;
	timer = Commands->Get_Random(Get_Float_Parameter("Min"),Get_Float_Parameter("Max"));
	Commands->Start_Timer(obj,this,timer,0);
}

void Neo_CreateObject_RandomTimer::Timer_Expired(GameObject *obj,int number)
{
	//Time's up,create the object

	GameObject *newobj;

	newobj = Commands->Create_Object(Get_Parameter("Preset_Name"),Commands->Get_Position(obj));
	Commands->Set_Facing(newobj,Commands->Get_Facing(obj));

	int repeat = Get_Int_Parameter("Repeat");

	if (repeat == 1) //Check if we should start again
	{
		//Start Counting again
		float timer;
		timer = Commands->Get_Random(Get_Float_Parameter("Min"),Get_Float_Parameter("Max"));
		Commands->Start_Timer(obj,this,timer,0);
	}
}

//****************Neo_Vehicle_AI*****************

void Neo_Vehicle_AI::Created(GameObject *obj)
{
	//Variable Initializing
	targetID = 0;
	position.X = 0;
	position.Y = 0;
	position.Z = 0;
	rampaging = false;
	guarding = false;
	moving = false;
	hunting = false;
//	move_priority = 90;
//	attack_priority = 100;
	face_priority = 1;
	current_priority = 10;
	guard_timer = 2;
	attack_timer = 2;
	mypos = Commands->Get_Position(obj);
	move_count = 0;

	//Standard AI Startup Junk
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Vehicle_Transitions(obj,false);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
}

void Neo_Vehicle_AI::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	/***********************************************/
	//				Message List
	/***********************************************/
	//				Incoming
	//	127127: Order to hunt at location of sender
	//	781781: Order to hunt a new player (sender)
	//	329329: Order to guard location of sender
	//	983983: Order to guard a new player (sender)
	//	param will be used to determine order priority
	/***********************************************/

	if (!rampaging) //Ignore orders if beserk
	{
		//Maybe a Switch statement would be better?
		if (type == 127127)
		{
			position = Commands->Get_Position(sender);
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,10);
			params.Set_Movement(position,1,2,false);
			params.MoveBackup = Set_Direction(obj);
			Commands->Action_Goto(obj,params);
			moving = true;

			//Offense code to be added here?
		}
		else if (type == 781781)
		{
			Hunt(obj,Set_Target(sender));
			//Offense code to be added here?
		}
		else if (type == 329329)
		{
			position = Commands->Get_Position(sender);
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,10);
			params.Set_Movement(position,1,2,false);
			params.MoveBackup = Set_Direction(obj);
			Commands->Action_Goto(obj,params);

			moving = true;
			guarding = true;
			hunting = false;
		}
		else if (type == 983983)
		{
			Guard(obj,Set_Target(sender));
			//Might need more here...
		}
	}
}

void Neo_Vehicle_AI::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	/**********************************************/
	//				Action IDs
	/**********************************************/
	//	10: Goto action
	//	20: Attack action
	//	30: Facing action
	/**********************************************/

	if (action_id == 10)
	{
		moving = false;
		move_count = 0; //move completed
	}
}

void Neo_Vehicle_AI::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	//This function needs to be rewritten from scratch

	int testID = Commands->Get_Player_Type(damager);
	int myID = Commands->Get_Player_Type(obj);

	if (testID != myID) //Did the enemy hit me?
	{
		if (!hunting && !guarding) //Not doing anything?
		{
			Hunt(obj,Set_Target(damager));
		}
		else if (hunting) //On a hunt?
		{
			//Might not need the Hunt function here...
			Hunt(obj,Select_Target(obj,damager));
			//This determines if damager bigger threat than target
		}
	}
	else if (!hunting && !guarding) //Am I doing anything?
	{
		Guard(obj,Set_Target(damager)); //must be a call for help
	}
}

void Neo_Vehicle_AI::Enemy_Seen(GameObject *obj,GameObject *enemy)
{
	if (guarding)
	{
		Vector3 pos = Commands->Get_Position(enemy);
		Vector3 mypos = Commands->Get_Position(obj);
		float distance = Commands->Get_Distance(pos,mypos);

		current_priority += 1; //override last command

		if (distance > Get_Float_Parameter("MaxRange"))
		{
			if (!moving)
			{
				ActionParamsStruct params;
				params.Set_Basic(this,(float)face_priority,30);
				params.Set_Face_Location(pos,5);
				Commands->Action_Face_Location(obj,params);
			}
		}
		else
		{
			//May need a 'Check_Target' that won't touch targetID
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,20);
			params.Set_Attack(enemy,distance,0,Set_Ammo(enemy));
			params.AttackCheckBlocked = true;
			Commands->Action_Attack(obj,params);
		}

		return; // we're done here
	}

	//If I'm not guarding,might as well go hunting

	Hunt(obj,Select_Target(obj,enemy));

}

void Neo_Vehicle_AI::Timer_Expired(GameObject *obj,int number)
{
	/***********************************************/
	//				Timer List
	/***********************************************/
	//	11: Guard Timer
	//	22: Hunt Timer
	/***********************************************/
	if (number == 11)
	{
		if (guarding) //in case guard was forced off
		{
			guarding = false; //guard will only continue if Guard called
			GameObject *target = Commands->Find_Object(targetID);

			if (target) //Check if the target lives
			{
				Guard(obj,Set_Target(target));
			}
			else //target dead
			{
				targetID = 0;
				current_priority = 10; //drop priority back down
			}
		}
	}
	else if (number == 22)
	{
		if (hunting) //In case hunt was forced off
		{
			hunting = false; //hunt will only continue if Hunt called
			GameObject *target = Commands->Find_Object(targetID);

			if (target) //Check if the target lives
				Hunt(obj,Set_Target(target));//Hunt starts a new timer by itself
			else
			{
				targetID = 0; //target dead,forget him
				current_priority = 10; //drop priority back down
			}
		}
	}
}

void Neo_Vehicle_AI::Sound_Heard(GameObject *obj,const CombatSound &sound)
{
	if (guarding)
	{
		if (!moving)
		{
			if (Commands->Is_A_Star(sound.Creator))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,(float)face_priority,30);
				params.Set_Face_Location(sound.Position,5);
				Commands->Action_Face_Location(obj,params);
			}
		}
	}
	else
	{
		if (!rampaging)
		{
			if (!moving)
			{
				if (Commands->Is_A_Star(sound.Creator))
				{
					ActionParamsStruct params;
					params.Set_Basic(this,(float)face_priority,30);
					params.Set_Face_Location(sound.Position,5);
					Commands->Action_Face_Location(obj,params);
				}
			}
		}
	}
}

void Neo_Vehicle_AI::Destroyed(GameObject *obj)
{
	//clean up things,send off messages
}

void Neo_Vehicle_AI::Register_Auto_Save_Variables()
{
	//Not sure on Size,all this could be wrong
	Auto_Save_Variable(&targetID,4,1);
	Auto_Save_Variable(&position,4,2);
	Auto_Save_Variable(&rampaging,4,3);
	Auto_Save_Variable(&guarding,4,4);
	Auto_Save_Variable(&moving,4,5);
	Auto_Save_Variable(&hunting,4,6);
//	Auto_Save_Variable(&move_priority,4,7);
//	Auto_Save_Variable(&attack_priority,4,8);
	Auto_Save_Variable(&face_priority,4,9);
	Auto_Save_Variable(&current_priority,4,10);
	Auto_Save_Variable(&guard_timer,4,11);
	Auto_Save_Variable(&attack_timer,4,12);
	Auto_Save_Variable(&mypos,4,13);
	Auto_Save_Variable(&move_count,4,14);
}

void Neo_Vehicle_AI::Hunt(GameObject *obj,GameObject *target)
{
	if (target) //Can't do anything without a target
	{
		hunting = true; //The hunt has begun!
		guarding = false; //W00T! no more guard duty!

		Vector3 pos = Commands->Get_Position(target);
		Vector3 mypos = Commands->Get_Position(obj);
		float distance = Commands->Get_Distance(pos,mypos);
		float max = Get_Float_Parameter("MaxRange");

		current_priority += 1; //override last hunt command

		if (distance > max)
		{
			float deviation;

			if (max > 10)
				deviation = max - 10;
			else
				deviation = max;

			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,10);
			params.Set_Movement(pos,1,deviation,false);
			params.MoveBackup = Set_Direction(obj);
			Commands->Action_Goto(obj,params);
			moving = true;
		}
		else
		{
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,20);
			params.Set_Attack(target,distance,0,Set_Ammo(target));
			params.AttackCheckBlocked = true;
			Commands->Action_Attack(obj,params);
		}

		Commands->Start_Timer(obj,this,attack_timer,22);
	}
}

void Neo_Vehicle_AI::Guard(GameObject *obj,GameObject *target)
{
	if (target) //Check for a target
	{
		guarding = true; //time for guard duty
		hunting = false; //any hunts have ended

		Vector3 pos = Commands->Get_Position(target);
		Vector3 mypos = Commands->Get_Position(obj);
		float distance = Commands->Get_Distance(pos,mypos);

		current_priority += 1; //override last command

		if (distance > 10)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,10);
			params.Set_Movement(pos,1,8,false);
			params.MoveBackup = Set_Direction(obj);
			Commands->Action_Goto(obj,params);
			moving = true;
		}

		Commands->Start_Timer(obj,this,guard_timer,11);
	}
}

GameObject *Neo_Vehicle_AI::Select_Target(GameObject *obj,GameObject *target)
{
	//This function determines threats between old and new targets
	GameObject *selected = 0; //if threat rating skips,this stays 0
							//if threat keeps old,this stays 0

	if (target) //First see if there is a valid target
	{
		GameObject *old = Commands->Find_Object(targetID);
		if (old) //is there a current target?
		{
			unsigned int newID = Commands->Get_ID(target);

			/******************Threat Rating System**********************/

			if (newID != targetID) //is this a new target
			{
				//requires threat assessment
				if (Commands->Is_A_Star(target)) //Is new one a player?
				{
					GameObject *vehicle = Get_Vehicle(target);

					if (vehicle) //is player in a tank?
					{
						if (Commands->Is_A_Star(old))
						{
							//Both are players,do a vehicle test
							vehicle = Get_Vehicle(old);
							if (vehicle) //is the old in a vehicle?
							{
								//both in vehicle,closer is priority
								selected = Get_Closest(obj,target,old);
							}
							else //old not in a vehicle
							{
								//specify distance to change on
								float distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(old));
								if (distance > Get_Float_Parameter("MaxRange"))
								{
									distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(target));
									if (distance < Get_Float_Parameter("MaxRange"))
										selected = target;
								}
							}
						}
						else
						{
							//new is a player(in tank),old is not
							if (old->As_SoldierGameObj())
							{
								//old is a soldier
								//switch to new target,old not worth it
								selected = target;
							}
							else if (old->As_VehicleGameObj())
							{
								//old is a vehicle,new is a player in tank
								//switch based on distance
								selected = Get_Closest(obj,target,old);
							}
						}
					}
					else //new player is on foot
					{
						if (Commands->Is_A_Star(old))
						{
							//Both are players,do a vehicle test
							vehicle = Get_Vehicle(old);
							if (vehicle) //is the old in a vehicle?
							{
								//new on foot,old in vehicle
								//specify distance to change on
								float distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(old));
								if (distance > Get_Float_Parameter("MaxRange"))
								{
									distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(target));
									if (distance < Get_Float_Parameter("MaxRange"))
										selected = target;
								}
							}
							else //old not in a vehicle
							{
								//both on foot,choose one closer
								selected = Get_Closest(obj,target,old);
							}
						}
						else
						{
							//new is a player on foot,old is not a player
							if (old->As_SoldierGameObj())
							{
								//old is a soldier
								//switch to new target,old not worth it
								selected = target;
							}
							else if (old->As_VehicleGameObj())
							{
								//old is a vehicle
								//switch if old out of range and new is close
								float distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(old));
								if (distance > Get_Float_Parameter("MaxRange"))
								{
									distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(target));
									if (distance < Get_Float_Parameter("MaxRange"))
										selected = target;
								}
							}
						}
					}
				}
				else if (Commands->Is_A_Star(old)) //Is the old guy a player?
				{
					//old a player,new not a player
					GameObject *vehicle = Get_Vehicle(old);
					if (vehicle) //is the old in a vehicle?
					{
						//old in vehicle
						if (target->As_SoldierGameObj())
						{
							//new is a soldier,old is player in a tank
							//Perhaps an out of range check here?
							selected = 0; //keep old					
						}
						else if (target->As_VehicleGameObj())
						{
							//new is a vehicle,old is player in a tank
							//switch if new closer
							selected = Get_Closest(obj,target,old);
						}
					}
					else //old on foot
					{
						if (target->As_SoldierGameObj())
						{
							//new is a soldier
							selected = Get_Closest(obj,target,old);
						}
						else if (target->As_VehicleGameObj())
						{
							//new is a vehicle,old just a player
							selected = target;
						}
					}
				}
			}
		}
		else	//old target was invalid
			targetID = 0; //forget ID
	}

	/***************Determine Specific Target*******************/

	if (selected) //Did we determine a new target?
	{
		targetID = Commands->Get_ID(selected); //remember target
		
		if (!hunting && !guarding) //only initiate if not doing anything
		{
			if (Commands->Is_A_Star(selected)) //Is it a player?
			{
				GameObject *vehicle = Get_Vehicle(selected); 

				if (vehicle) //Are they in a vehicle
					return vehicle; //if so,return vehicle
				else
					return selected; //if not,return player
			}
			return selected; //nothing left but the target
		}
	}
	else //No new target was picked,only change if not doing anything
	{
		if (!hunting && !guarding) //only initiate if not doing anything
		{
			targetID = Commands->Get_ID(target); //remember target
			if (Commands->Is_A_Star(target)) //Is it a player?
			{
				GameObject *vehicle = Get_Vehicle(target); 

				if (vehicle) //Are they in a vehicle
					return vehicle; //if so,return vehicle
				else
					return target; //if not,return player
			}
			return target; //nothing left but the target
		}
	}

	return 0; //don't start new job if already working
}

GameObject *Neo_Vehicle_AI::Set_Target(GameObject *target)
{
	//This function sets a new target even if one exists
	targetID = Commands->Get_ID(target); //remember target

	if (!hunting && !guarding) //only initiate if not doing anything
	{
		if (Commands->Is_A_Star(target)) //Is it a player?
		{
			GameObject *vehicle = Get_Vehicle(target); 

			if (vehicle) //Are they in a vehicle
				return vehicle; //if so,return vehicle
			else
				return target; //if not,return player
		}

		return target; //nothing left but the target
	}

	return 0; //don't start new job if already working
}

GameObject *Neo_Vehicle_AI::Get_Closest(GameObject *obj,GameObject *new_target,GameObject *old_target)
{
	Vector3 obj_pos = Commands->Get_Position(obj);
	Vector3 target1_pos = Commands->Get_Position(new_target);
	Vector3 target2_pos = Commands->Get_Position(old_target);

	float dist1 = Commands->Get_Distance(obj_pos,target1_pos);
	float dist2 = Commands->Get_Distance(obj_pos,target2_pos);

	if (dist1 < dist2)
		return new_target; //new is closer,inform of change
	else
		return 0; //old is closer,no change needed
}

bool Neo_Vehicle_AI::Set_Ammo(GameObject *target)
{
	//This function sets ammo type to use on a target

	if (target)//Just to be on the safe side
	{
		if (target->As_SoldierGameObj()) //is the target a soldier?
		{
			if (Get_Int_Parameter("vsSoldier")) 
				return true; //Use Primary
			else
				return false; //Use Secondary
		}

		if (target->As_VehicleGameObj())
		{
			if (Get_Vehicle_Mode(target) != VEHICLE_TYPE_FLYING)
			{
				if (Get_Int_Parameter("vsAircraft")) 
					return true; //Use Primary
				else
					return false; //Use Secondary
			}
			else
			{
				if (Get_Int_Parameter("vsVehicle")) 
					return true; //Use Primary
				else
					return false; //Use Secondary
			}
		}
	}
	return true; //If all else fails,use Primary Ammo
}

bool Neo_Vehicle_AI::Set_Direction(GameObject *obj)
{
	Vector3 current = Commands->Get_Position(obj);
	float distance = Commands->Get_Distance(mypos,current);

	if (distance < 1) //did I barely move?
	{
		if (move_count > 2)
		{
			move_count = 0;
			return true; //tried at least 3 times,reverse direction
		}
		move_count++; //haven't hit the limit yet
	}

	mypos = current; //remember current position
	return false; //Don't drive backwards
}

//**************Script Registrants***************

ScriptRegistrant<Neo_CreateObject_RandomTimer> Neo_CreateObject_RandomTimer_Registrant("Neo_CreateObject_RandomTimer","Preset_Name=blank:string,Min=0.0:float,Max=60.0:float,Repeat=1:int");
ScriptRegistrant<Neo_Vehicle_AI> Neo_Vehicle_AI_Registrant("Neo_Vehicle_AI","MaxRange=0.0:float,vsSoldier=1:int,vsVehicle=1:int,vsAircraft=1:int");
