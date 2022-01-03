#pragma once
#include "scripts.h"


#define MS_BASE                                          0xDA000000

#define CUSTOM                                           0x00010000
#define TIMER                                            0x00020000
#define ACTION                                           0x00030000

#define CUSTOM_TARGETPAINTER_TARGET_OBJECT_CREATED       (MS_BASE|CUSTOM|100201)
#define CUSTOM_TARGETPAINTER_TARGET_OBJECT_DESTROY_SELF  (MS_BASE|CUSTOM|100202)


#define CUSTOM_AI_SOLDIER_DIED                           (MS_BASE|CUSTOM|100501)
#define CUSTOM_AI_VEHICLE_DIED                           (MS_BASE|CUSTOM|100502)
#define CUSTOM_AI_GO_TO_VEHICLE                          (MS_BASE|CUSTOM|100503)
#define CUSTOM_AI_ARRIVED_TO_VEHICLE                     (MS_BASE|CUSTOM|100504)
#define CUSTOM_AI_VEHICLE_REQUEST_ENTER                  (MS_BASE|CUSTOM|100505)
#define CUSTOM_AI_VEHICLE_EXITED                         (MS_BASE|CUSTOM|100506)
#define CUSTOM_AI_REQUEST_REFUND                         (MS_BASE|CUSTOM|100507)
#define CUSTOM_AI_TRANSPORT_ARRIVED                      (MS_BASE|CUSTOM|100508)
#define CUSTOM_AI_VEHICLE_AMMO_EMPTY                     (MS_BASE|CUSTOM|100509)
#define CUSTOM_AI_CHECK_REPAIR_NEEDED                    (MS_BASE|CUSTOM|100510)
#define CUSTOM_AI_GO_TO_REFILL                           (MS_BASE|CUSTOM|100511)
#define CUSTOM_AI_GO_TO_RELOAD                           (MS_BASE|CUSTOM|100512)
#define CUSTOM_AI_GO_TO_EXIT_VEHICLE                     (MS_BASE|CUSTOM|100513)
#define CUSTOM_AI_ARRIVED_TO_REFILL                      (MS_BASE|CUSTOM|100514)
#define CUSTOM_AI_ARRIVED_TO_EXIT_VEHICLE                (MS_BASE|CUSTOM|100515)// TODO: use generic ARRIVED_TO_SECONDARY_TASK custom.
#define CUSTOM_AI_REFILL_COMPLETE                        (MS_BASE|CUSTOM|100516)
#define CUSTOM_AI_SEARCH_FOR_VEHICLE                     (MS_BASE|CUSTOM|100517)
#define CUSTOM_AI_DEACTIVATE_AI                          (MS_BASE|CUSTOM|100518)
#define CUSTOM_AI_GO_SWITCH_OBJECTIVE                    (MS_BASE|CUSTOM|100519)
#define CUSTOM_AI_REQUEST_PATROL_POINT                   (MS_BASE|CUSTOM|100520)
#define CUSTOM_AI_GO_TO_PATROL_POINT                     (MS_BASE|CUSTOM|100521)
#define CUSTOM_AI_CHECK_GO_HOME                          (MS_BASE|CUSTOM|100522)
#define CUSTOM_AI_SEND_PUBLIC_MESSAGE                    (MS_BASE|CUSTOM|100523)
#define CUSTOM_AI_ADD_RADAR_TARGET                       (MS_BASE|CUSTOM|100524)
#define CUSTOM_AI_CHECK_RADAR_TARGETS                    (MS_BASE|CUSTOM|100525)
#define CUSTOM_AI_ENEMY_KILLED                           (MS_BASE|CUSTOM|100526)
#define CUSTOM_AI_ADD_ALLY_TARGET                        (MS_BASE|CUSTOM|100527)
#define CUSTOM_AI_REPAIR_NEEDED                          (MS_BASE|CUSTOM|100528)
#define CUSTOM_AI_REPAIR_COMPLETE                        (MS_BASE|CUSTOM|100529)

#define CUSTOM_AI_COOP_NEXT_RALLYPOINT                   (MS_BASE|CUSTOM|100601)
#define CUSTOM_AI_COOP_ARRIVED_TO_RALLYPOINT             (MS_BASE|CUSTOM|100602)
#define CUSTOM_AI_COOP_GROUPRUSH_COMPLETE                (MS_BASE|CUSTOM|100603)
#define CUSTOM_AI_COOP_STOP_SECONDARY_OBJECTIVE          (MS_BASE|CUSTOM|100604)

#define CUSTOM_AI_COOP_PLAYER_RADIO_MESSAGE              (MS_BASE|CUSTOM|100701)
#define CUSTOM_AI_COOP_PLAYER_FOLLOW                     (MS_BASE|CUSTOM|100702)
#define CUSTOM_AI_COOP_PLAYER_FOLLOW_COVER               (MS_BASE|CUSTOM|100703)
#define CUSTOM_AI_COOP_PLAYER_FOLLOW_STOP                (MS_BASE|CUSTOM|100704)
#define CUSTOM_AI_COOP_PLAYER_DEFEND_BASE                (MS_BASE|CUSTOM|100705)
#define CUSTOM_AI_COOP_PLAYER_CREATE_OBJECTIVE           (MS_BASE|CUSTOM|100706)
#define CUSTOM_AI_COOP_PLAYER_DESTROY_OBJECTIVE          (MS_BASE|CUSTOM|100707)

#define CUSTOM_AI_ORDER_MOVE                             (MS_BASE|CUSTOM|100801)
#define CUSTOM_AI_ORDER_ATTACK                           (MS_BASE|CUSTOM|100802)
#define CUSTOM_AI_ORDER_DEFEND                           (MS_BASE|CUSTOM|100803)

#define TIMER_AI_CONTROLLER_PLAYERCHECK                  (MS_BASE|TIMER|100501)
#define TIMER_AI_OBJECT_SPAWN_COUNTDOWN                  (MS_BASE|TIMER|100502)
#define TIMER_AI_OBJECT_SPAWN_GHOSTING_COUNTDOWN         (MS_BASE|TIMER|100503)
#define TIMER_AI_OBJECT_ACTIVATE_COUNTDOWN               (MS_BASE|TIMER|100504)
#define TIMER_AI_WAIT_FOR_VEHICLE_CONSTRUCT              (MS_BASE|TIMER|100505)
#define TIMER_AI_GO_TO_VEHICLE_TIMEOUT                   (MS_BASE|TIMER|100506)
#define TIMER_AI_UNDER_ATTACK                            (MS_BASE|TIMER|100507)
#define TIMER_AI_REQUEST_DEPLOY                          (MS_BASE|TIMER|100508)
#define TIMER_AI_POST_CREATED                            (MS_BASE|TIMER|100509)
#define TIMER_AI_SEARCH_FOR_VEHICLE                      (MS_BASE|TIMER|100510)
#define TIMER_AI_REFILL_COOLDOWN                         (MS_BASE|TIMER|100511)
#define TIMER_AI_DEFEND                                  (MS_BASE|TIMER|100512)

#define TIMER_AI_CHECK_FOR_BLOCK                         (MS_BASE|TIMER|100551)
#define TIMER_AI_CHECK_GET_AROUND_TIMEOUT                (MS_BASE|TIMER|100552)
#define TIMER_AI_CHECK_UNSTUCK_TIMEOUT                   (MS_BASE|TIMER|100553)
#define TIMER_AI_CHECK_DISTANCE                          (MS_BASE|TIMER|100554)
#define TIMER_AI_CHECK_RADAR                             (MS_BASE|TIMER|100555)
#define TIMER_AI_CHECK_ALLIES                            (MS_BASE|TIMER|100556)

#define TIMER_AI_COOP_RETRY_REQUEST                      (MS_BASE|TIMER|100601)
#define TIMER_AI_COOP_GROUP_WAIT_TEAM_0                  (MS_BASE|TIMER|100602)
#define TIMER_AI_COOP_GROUP_WAIT_TEAM_1                  (MS_BASE|TIMER|100603)
#define TIMER_AI_COOP_PAUSE_INITIATION                   (MS_BASE|TIMER|100604)

#define ACTION_AI_GO_TO_VEHICLE                          (MS_BASE|ACTION|100001)
#define ACTION_AI_GET_AROUND_REVERSE                     (MS_BASE|ACTION|100002)
#define ACTION_AI_GET_AROUND_FORWARD                     (MS_BASE|ACTION|100003)
#define ACTION_AI_RANDOM_MOVING                          (MS_BASE|ACTION|100004)

#define DEFAULT_BOT_NAME                                 "Bot"

#define MS_RGB_JOINLEFT_MSG                              178,178,178     // grey
#define MS_RGB_DEBUG                                     255,255,255

class Radio_Message_Type
{
public:
	static int BUILDING_NEEDS_REPAIRS;
	static int GET_IN_THE_VEHICLE;
	static int GET_OUT_OF_THE_VEHICLE;
	static int DESTROY_THAT_VEHICLE;
	static int DONT_GET_IN_MY_WAY;
	static int AFFIRMATIVE;
	static int NEGATIVE;

	static int TAKE_THE_POINT;
	static int MOVE_OUT;
	static int FOLLOW_ME;
	static int COVER_ME;
	static int DESTROY_IT_NOW;

	static int ATTACK_THE_HARVESTER;
	static int ATTACK_THAT_STRUCTURE;
	static int DEFEND_THE_BASE;
	static int DEFEND_THE_HARVESTER;
	static int DEFEND_THAT_STRUCTURE;
};


class MS_AccessHelper
{
public:
	static void Init_AI_Global_Controller();
	static int Get_AI_Player_Controllers_Count(int team);
	static void Give_Bot_Credits(int team, float amount);
	static float Steal_Bot_Credits(int team, float percentage);
	static void Receive_Radio_Command(GameObject *sender, int AnnouncementID, int orderType = 0, int orderObjID = 0);
	static void Send_Radio_Order_To_Bot(GameObject *sender, GameObject *receiver, int radioIndex, int orderType = 0, int orderObjID = 0);
	static float Get_Money(GameObject *object);
	static void Give_Money(GameObject *object, float money);
	static void Activate_Defense_Protocol(int team);
};


class MS_AI_Rally_Point : public ScriptImpClass
{
	void Created(GameObject *obj) override;
	void Destroyed(GameObject *obj) override;
public:
	int Get_Team();
	int Get_NodeGroup();
	int Get_NodeNum();
	bool Get_IsInfantryOnly();
	GameObject *Get_Object();
};

class MS_AI_Patrol_Point : public MS_AI_Rally_Point
{
	void Created(GameObject *obj) override;
	void Destroyed(GameObject *obj) override;
};
