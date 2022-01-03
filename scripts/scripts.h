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
#ifndef SCRIPTS_INCLUDE__SCRIPTS_H
#define SCRIPTS_INCLUDE__SCRIPTS_H
#include "Vector3.h"

#ifndef _MSC_VER
#error The renegade scripts.dll requires Microsoft Visual C++ in order to work
#endif

class ScriptableGameObj;
typedef ScriptableGameObj GameObject;
class ScriptSaver;
class ScriptLoader;
class ScriptFactory;
class ScriptClass;

enum ActionCompleteReason {
	ACTION_COMPLETE_NORMAL,
	ACTION_COMPLETE_LOW_PRIORITY,
	ACTION_COMPLETE_PATH_BAD_START,
	ACTION_COMPLETE_PATH_BAD_DEST,
	ACTION_COMPLETE_MOVE_NO_PROGRESS_MADE,
	ACTION_COMPLETE_ATTACK_OUT_OF_RANGE,
	ACTION_COMPLETE_CONVERSATION_ENDED,
	ACTION_COMPLETE_CONVERSATION_INTERRUPTED,
	ACTION_COMPLETE_CONVERSATION_UNABLE_TO_INIT,
	MOVEMENT_COMPLETE_ARRIVED,
};

class ScriptVariableClass {
public:
	ScriptVariableClass( void * data_ptr, int data_size, int id, ScriptVariableClass * next ) : DataPtr( data_ptr ), DataSize( data_size ),ID( id ),Next( next )	{}
	void *Get_Data_Ptr()
	{
		return DataPtr;
	}
	int	Get_Data_Size()
	{
		return DataSize;
	}
	int Get_ID()
	{
		return ID;
	}
	ScriptVariableClass *Get_Next()
	{
		return Next;
	}
private:
	void *DataPtr;
	int	DataSize;
	int	ID;
	ScriptVariableClass *Next;
};
enum CombatSoundType {
	SOUND_TYPE_NONE,
	SOUND_TYPE_OLD_WEAPON,
	SOUND_TYPE_FOOTSTEPS,
	SOUND_TYPE_VEHICLE,
	SOUND_TYPE_GUNSHOT,
	SOUND_TYPE_BULLET_HIT,
	NUM_DEFINED_SOUND_TYPES,
	SOUND_TYPE_DESIGNER01 = 991,
	SOUND_TYPE_DESIGNER02,
	SOUND_TYPE_DESIGNER03,
	SOUND_TYPE_DESIGNER04,
	SOUND_TYPE_DESIGNER05,
	SOUND_TYPE_DESIGNER06,
	SOUND_TYPE_DESIGNER07,
	SOUND_TYPE_DESIGNER08,
	SOUND_TYPE_DESIGNER09,
};
struct CombatSound {
	Vector3 Position; //this is passed from the position parameter of Create_Logical_Sound
	int sound; //this is passed from the sound parameter of Create_Logical_Sound
	ScriptableGameObj *Creator; //this is passed from the obj parameter of Create_Logical_Sound
};

class SCRIPTS_API GameObjObserverClass {
public:
	GameObjObserverClass() : ID( 0 ) {}
	virtual	~GameObjObserverClass() {}
	virtual const char *Get_Name() = 0;
	void Set_ID(int id)
	{
		ID = id;
	}
	int Get_ID()
	{
		return ID;
	}
	virtual void Attach(GameObject *obj) = 0;
	virtual void Detach(GameObject *obj) = 0;
	virtual void Created(GameObject *obj) = 0;
	virtual void Destroyed(GameObject *obj) = 0;
	virtual void Killed(GameObject *obj,GameObject *killer) = 0;
	virtual void Damaged(GameObject *obj,GameObject *damager,float amount) = 0;
	virtual void Custom(GameObject *obj,int type,int param,GameObject *sender) = 0;
	virtual void Sound_Heard(GameObject *obj,const CombatSound & sound) = 0;
	virtual void Enemy_Seen(GameObject *obj,GameObject *enemy) = 0;
	virtual void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason) = 0;
	virtual void Timer_Expired(GameObject *obj,int number) = 0;
	virtual void Animation_Complete(GameObject *obj,const char *animation_name) = 0;
	virtual void Poked(GameObject *obj,GameObject *poker) = 0;
	virtual void Entered(GameObject *obj,GameObject *enterer) = 0;
	virtual void Exited(GameObject *obj,GameObject *exiter) = 0;
	virtual GameObject* Owner() {return 0;}
protected:
	int ID;
};

class SCRIPTS_API ScriptClass : public GameObjObserverClass {
public:
	virtual ~ScriptClass() {}
	virtual GameObject** Get_Owner_Ptr() = 0;
	virtual void Set_Parameters_String(const char *params) = 0;
	virtual void Get_Parameters_String(char *buffer,unsigned int size) = 0;
	virtual void Save(ScriptSaver& saver) = 0;
	virtual void Load(ScriptLoader& loader) = 0;
};

class SCRIPTS_API ScriptImpClass : public ScriptClass {
public:
	static void Set_Request_Destroy_Func(void (*function)(ScriptClass*));
	void SetFactory(ScriptFactory* factory)
	{
		mFactory = factory;
	}
	ScriptImpClass();
	int CheckPlayerType(GameObject *obj,int type);
	void	Auto_Save_Variable( void * data_ptr, int data_size, int id );
	virtual ~ScriptImpClass();
	const char* Get_Name(void);

  /*! Script event that is triggered when this script is created
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  */
  void Created(GameObject *obj) {}

  /*! Script event that is triggered when the object gets destroyed
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  */
  void Destroyed(GameObject *obj) {}

  /*! Script event that is triggered when the object is killed. Note that this event will not be
  * triggered if the object is destroyed without being killed via loss of health, such as by another
  * script calling ScriptCommands::Destroy_Object()
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] killer Pointer to the GameObject that killed our object. NULL for environment damage
  */
  void Killed(GameObject *obj,GameObject *killer) {}

  /*! Script event that is triggered when the object takes damage
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] damager
  *    Pointer to the GameObject that damaged our object, which will be the shooter and not the
  *    vehicle they are driving (if any). NULL for environment damage
  * \param[in] amount The amount of damage that was done to the object
  */
  void Damaged(GameObject *obj,GameObject *damager,float amount) {}
  
  /*! Script event that is triggered when the object recieves a custom, which could have been sent
  * by another script or be triggered by a system event.
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] type The type of custom that has been recieved
  * \param[in] param The parameter that was attached to the custom by the sender
  * \param[in] sender Pointer to the GameObject from which this custom was sent
  */
  void Custom(GameObject *obj,int type,int param,GameObject *sender) {}

  /*! Script event that is triggered when the object hears a sound. Not currently well understood.
  * Presumably requires innate AI to be enabled and for the object to have a listener scale of >0
  * defined in it's preset definition.
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] sound Information about the sound that has been heard
  */
  void Sound_Heard(GameObject *obj,const CombatSound & sound) {}

  /*! Script event that is triggered when the object sees an enemy. This requires innate AI to be
  * enabled and for the object to have a sight range of >0 defined in its preset definition.
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] enemy Pointer to the enemy GameObject that was seen by the object
  */
  void Enemy_Seen(GameObject *obj,GameObject *enemy) {}

  /*! Script event that is triggered when the object completes an action that this script is
  * defined as the observer for.
  *
  * \see ActionParamsStruct
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] action_id ID of the action that was completed
  * \param[in] complete_reason The reason the action was marked as complete
  */
  void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason) {}

  /*! Script event that is triggered when a script defined timer expires
  *
  * \see ScriptCommands::Start_Timer()
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] number IF of the timer that expired
  */
  void Timer_Expired(GameObject *obj,int number) {}


  /*! Script event that is triggered when the object completes an animation
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] animation_name Name of the animation that was completed
  */
  void Animation_Complete(GameObject *obj,const char *animation_name) {}

  /*! Script event that is triggered when the object has been poked by a player. Requires bandtest.dll
  * to work correctly in multiplayer mode.
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] poker Pointer to the GameObject of the player that poked this object
  */
  void Poked(GameObject *obj,GameObject *poker) {}

  /*! Script event that is triggered on a script zone when an object enters its defined area
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] enterer Pointer to the GameObject of the object that entered this zone
  */
  void Entered(GameObject *obj,GameObject *enterer) {}
  
  /*! Script event that is triggered on a script zone when an object leaves its defined area
  *
  * \param[in] obj Pointer to the GameObject this script is attached to
  * \param[in] exiter Pointer to the GameObject of the object that left this zone
  */
  void Exited(GameObject *obj,GameObject *exiter) {}

  GameObject* Owner()
	{
		return mOwner;
	}
	GameObject** Get_Owner_Ptr()
	{
		return &mOwner;
	}
	void Set_Parameters_String(const char* params);
	void Get_Parameters_String(char* buffer, unsigned int size);
	virtual void Save_Data(ScriptSaver& saver) {}
	virtual void Load_Data(ScriptLoader& loader) {}
	virtual	void Register_Auto_Save_Variables()	{}
	const char* Get_Parameter(const char* name);
	const char* Get_Parameter(int index);
	int Get_Int_Parameter(const char* parameterName);
	float Get_Float_Parameter(const char* parameterName);
	Vector3 Get_Vector3_Parameter(int index);
	Vector3 Get_Vector3_Parameter(const char* parameterName);
	int Get_Parameter_Index(const char* parameterName);
	int Get_Parameter_Count()
	{
		return mArgC;
	}
	int Get_Int_Parameter(int index)
	{
		return atoi(Get_Parameter(index));
	}
	float Get_Float_Parameter(int index)
	{
		return (float)atof(Get_Parameter(index));
	}
    bool Get_Bool_Parameter(const char* name)
    {
        return Get_Int_Parameter(name) != 0;
    }
    bool Get_Bool_Parameter(int index)
    {
        return Get_Int_Parameter(index) != 0;
    }
	void Destroy_Script(void);
protected:
	void Attach(GameObject* obj);
	void Detach(GameObject* obj);
	virtual void Save(ScriptSaver& saver);
	virtual void Load(ScriptLoader& loader);
private:
	static void (*Request_Destroy_Script)(ScriptClass*);
	void Clear_Parameters(void);
	void Set_Parameter(int index, const char* str);
	GameObject* mOwner;
	int mArgC;
	char** mArgV;
	ScriptFactory *mFactory;
	ScriptVariableClass *AutoVariableList;
};
enum SoldierAIState {
	NO_AI_STATE_CHANGE = -1,
	AI_STATE_IDLE = 0,
	AI_STATE_SECONDARY_IDLE,
	AI_STATE_SEARCH,
	AI_STATE_COMBAT
};
const float	DONT_MOVE_ARRIVED_DIST	= 1000.0F;

class ActionParamsStruct {
public:
	ActionParamsStruct( void );
	void Set_Basic( GameObjObserverClass * script, float priority, int action_id, SoldierAIState ai_state = NO_AI_STATE_CHANGE ) { ObserverID = script->Get_ID(); Priority = (int)priority; ActionID = action_id; AIState = ai_state; }
	void Set_Basic( long observer_id, float priority, int action_id, SoldierAIState ai_state = NO_AI_STATE_CHANGE ) { ObserverID = observer_id; Priority = (int)priority; ActionID = action_id; AIState = ai_state; }
	void Set_Look( const Vector3 & location, float duration ) { LookLocation = location; LookDuration = duration; }	
	void Set_Look( GameObject * object, float duration ) { LookObject = object; LookDuration = duration; }
	void Set_Look( const Vector3 & obj_pos, float angle,  float duration );
	void Set_Movement( const Vector3 & location, float speed, float arrived_distance, bool crouched = false ) { MoveLocation = location; MoveSpeed = speed; MoveArrivedDistance = arrived_distance; MoveCrouched = crouched; }
	void Set_Movement( GameObject *	object, float speed, float arrived_distance, bool crouched = false ) { MoveObject = object; MoveSpeed = speed; MoveArrivedDistance = arrived_distance; MoveCrouched = crouched; }
	void Set_Attack( const Vector3 & location, float range, float error, bool primary_fire ) { AttackLocation = location; AttackRange = range; AttackError = error; AttackPrimaryFire = primary_fire; }
	void Set_Attack( GameObject * object, float range, float error, bool primary_fire ) { AttackObject = object; AttackRange = range; AttackError = error; AttackPrimaryFire = primary_fire; }
	void Set_Animation( const char * name, bool looping ) { AnimationName = name; AnimationLooping = looping; }
	void Set_Face_Location( const Vector3 &location, float duration )	{ FaceLocation = location; FaceDuration = duration; }
	void Set_Face_Location(  const Vector3 & obj_pos, float angle, float duration );
	void Join_Conversation( int active_conversation_id )	{ ActiveConversationID = active_conversation_id; }
	void Start_Conversation( const char * name )			{ ConversationName = name; }
	void Dock_Vehicle( const Vector3 &dock_location, const Vector3 &dock_entrance ) { DockLocation = dock_location; DockEntrance = dock_entrance; }

  /*! Priority for this action, this determines if it will override an existing action on the target
  * object. If the priority of the existing action is lower, or there is no existing action, this
  * action will be executed. If the priority is lower than that of the existing action it will be
  * ignored. */
  int Priority;
  int ActionID;                 //!< An identifier for this action, passed to ScriptImpClass::Action_Complete
  long ObserverID;              //!< UID of a script to recieve notifications about this action, such as ScriptImpClass::Action_Complete
  Vector3 LookLocation;
  GameObject *LookObject;
  float LookDuration;
  Vector3 MoveLocation;         //!< A target location for this object to attempt to reach
  GameObject *MoveObject;
  Vector3 MoveObjectOffset;
  float MoveSpeed;
  float MoveArrivedDistance;    //!< How close to the target location the object must get to be considered as arrived
  bool MoveBackup;              //!< True to drive in reverse, false to drive forwards
  bool MoveFollow;
  bool MoveCrouched;
  /*! True to use the global pathfind data to assist in route planning, false to ignore it
  * (presumably only useful if you specify a waypath to follow) */
  bool MovePathfind;
  bool ShutdownEngineOnArrival; //!< True to shutdown engine sounds upon arrival, false to leave them enabled
  float AttackRange;            //!< The maximum range at which the object should attack an enemy
  float AttackError;            //!< Apply up to this much error to the turret aim. If this is 0 the aim will be perfect
  bool AttackErrorOverride;
  GameObject *AttackObject;     //!< An object to track the movement of and fire at (if within range)
  bool AttackPrimaryFire;
  bool AttackCrouched;
  Vector3 AttackLocation;       //!< A location to fire at, presumably ignored if AttackObject is not NULL
  bool AttackCheckBlocked;
  bool AttackActive;
  bool AttackWanderAllowed;
  bool AttackFaceTarget;
  bool AttackForceFire;
  bool ForceFacing;
  Vector3 FaceLocation;
  float FaceDuration;
  bool IgnoreFacing;
  int	WaypathID;
  int WaypointStartID;
  int WaypointEndID;
  bool WaypathSplined;
  const char *AnimationName;
  bool AnimationLooping;
  int ActiveConversationID;
  const char *ConversationName;
  SoldierAIState AIState;
  Vector3 DockLocation;
  Vector3 DockEntrance;
};

inline ActionParamsStruct::ActionParamsStruct( void ) :
	Priority( 0 ),
	ActionID( 0 ),
	ObserverID( 0 ),
	LookLocation( 0,0,0 ),
	LookObject( NULL ),
	LookDuration( 0 ),
	MoveLocation( 0,0,0 ),
	MoveObject( NULL ),
	MoveObjectOffset( 0,0,0 ),
	MoveSpeed( 1 ),
	MoveArrivedDistance( DONT_MOVE_ARRIVED_DIST ),
	MoveBackup( false ),
	MoveFollow( false ),
	MoveCrouched( false ),
	MovePathfind( true ),
	ShutdownEngineOnArrival( false ),
	AttackRange( 20 ),
	AttackError( 0 ),
	AttackErrorOverride( false ),
	AttackObject( NULL ),
	AttackPrimaryFire( true ),
	AttackCrouched( false ),
	AttackLocation( 0,0,0 ),
	AttackCheckBlocked( true ),
	AttackActive( true ),
	AttackWanderAllowed( false ),
	AttackFaceTarget( true ),
	AttackForceFire( false ),
	ForceFacing( false ),
	IgnoreFacing( false ),
	FaceLocation( 0, 0, 0 ),
	FaceDuration( 2.0F ),
	WaypathID( 0 ),
	WaypointStartID( 0 ),
	WaypointEndID( 0 ),
	WaypathSplined( false ),
	AnimationName( NULL ),
	AnimationLooping( false ),
	ActiveConversationID( 0 ),
	ConversationName( NULL ),
	AIState( NO_AI_STATE_CHANGE ),
	DockLocation( 0,0,0 ),
	DockEntrance( 0,0,0 )
{
}

void	inline ActionParamsStruct::Set_Face_Location( const Vector3 &obj_pos, float angle, float duration )
{
	FaceLocation	= obj_pos;
	FaceLocation.X	+= ::cos( angle );
	FaceLocation.Y	+= ::sin( angle );
	FaceDuration	= duration;
	return ;
}


void	inline ActionParamsStruct::Set_Look( const Vector3 &obj_pos, float angle,  float duration )
{
	LookLocation	= obj_pos;
	LookLocation.X	+= ::cos( angle );
	LookLocation.Y	+= ::sin( angle );
	LookDuration	= duration;
	return ;
}

enum {
	CUSTOM_EVENT_SYSTEM_FIRST = 1000000000,
	CUSTOM_EVENT_SOUND_ENDED,                                                           //!< Sent when sound has ended
	CUSTOM_EVENT_BUILDING_POWER_CHANGED,                                                //!< Sent when building power changes
	CUSTOM_EVENT_DOCK_BACKING_IN,                                                       //!< Sent when vehicle arrives at dock
	CUSTOM_EVENT_CINEMATIC_SET_FIRST_SLOT = 10000,                                      //!< Sent to set cinematic slot
	CUSTOM_EVENT_CINEMATIC_SET_LAST_SLOT = CUSTOM_EVENT_CINEMATIC_SET_FIRST_SLOT + 39,  //!< Sent to set cinematic slot
	CUSTOM_EVENT_POWERUP_GRANTED = 1000000025,                                          //!< Sent when powerup is granted
	CUSTOM_EVENT_BUILDING_DAMAGED,                                                      //!< Sent when building is damaged
	CUSTOM_EVENT_BUILDING_REPAIRED,                                                     //!< Sent when building is repaired
	CUSTOM_EVENT_VEHICLE_ENTERED,                                                       //!< Sent when vehicle is entered
	CUSTOM_EVENT_VEHICLE_EXITED,                                                        //!< Sent when vehicle is exited
	CUSTOM_EVENT_ATTACK_ARRIVED,                                                        //!< Sent on attack arrived
	CUSTOM_EVENT_CONVERSATION_BEGAN,                                                    //!< Sent on conversation begin
	CUSTOM_EVENT_CONVERSATION_REMARK_STARTED,                                           //!< Sent on conversation remark started
	CUSTOM_EVENT_CONVERSATION_REMARK_ENDED,                                             //!< Sent on conversation remark ended
	CUSTOM_EVENT_LADDER_OCCUPIED,                                                       //!< Sent on ladder occupied
	CUSTOM_EVENT_FALLING_DAMAGE,                                                        //!< Sent on falling damage
	CUSTOM_EVENT_VEHICLE_OWNER,                                                         //!< Special custom one sent by the new ownership code in bandtest.dll and by the scripts in mdbevf.cpp
	CUSTOM_EVENT_VEHICLE_DRIVER,                                                        //!< Special custom one sent to vehicles where the driver/owner gets out to trigger it to find out its driver
	CUSTOM_EVENT_VHEICLE_DRIVER_ID,                                                     //!< Response to CUSTOM_EVENT_VEHICLE_DRIVER, passes the owner GameObjects ID as the param
	CUSTOM_EVENT_BEACON_DEPLOYED,                                                       //!< Sent when a beacon is deployed
	CUSTOM_EVENT_REFILL,                                                                //!< Sent on refill at a PT
	CUSTOM_EVENT_BUILDING_REVIVED,														//!< Sent when building is revived
	CUSTOM_EVENT_OWNED_VEHICLE,                                                         //!< Special custom sent to the owner of a new vehicle so they can find their vehicle
};

enum {
	OBJECTIVE_TYPE_PRIMARY = 1,
	OBJECTIVE_TYPE_SECONDARY,
	OBJECTIVE_TYPE_TERTIARY,
	OBJECTIVE_STATUS_PENDING = 0,
	OBJECTIVE_STATUS_ACCOMPLISHED,
	OBJECTIVE_STATUS_FAILED,
	OBJECTIVE_STATUS_HIDDEN,
	RADAR_BLIP_SHAPE_NONE = 0,
	RADAR_BLIP_SHAPE_HUMAN,
	RADAR_BLIP_SHAPE_VEHICLE,
	RADAR_BLIP_SHAPE_STATIONARY,
	RADAR_BLIP_SHAPE_OBJECTIVE,
	RADAR_BLIP_SHAPE_EXTRA_1,
	RADAR_BLIP_SHAPE_EXTRA_2,
	RADAR_BLIP_SHAPE_EXTRA_3,
	RADAR_BLIP_SHAPE_EXTRA_4,
	RADAR_BLIP_SHAPE_EXTRA_DISTANT_1,
	RADAR_BLIP_SHAPE_EXTRA_DISTANT_2,
	RADAR_BLIP_SHAPE_EXTRA_DISTANT_3,
	RADAR_BLIP_SHAPE_EXTRA_DISTANT_4,
	RADAR_BLIP_COLOR_NOD = 0,
	RADAR_BLIP_COLOR_GDI,
	RADAR_BLIP_COLOR_NEUTRAL,
	RADAR_BLIP_COLOR_MUTANT,
	RADAR_BLIP_COLOR_RENEGADE,
	RADAR_BLIP_COLOR_PRIMARY_OBJECTIVE,
	RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE,
	RADAR_BLIP_COLOR_TERTIARY_OBJECTIVE,
	SCRIPT_PLAYERTYPE_SPECTATOR	= -4,
	SCRIPT_PLAYERTYPE_MUTANT,
	SCRIPT_PLAYERTYPE_NEUTRAL,
	SCRIPT_PLAYERTYPE_RENEGADE,
	SCRIPT_PLAYERTYPE_NOD,
	SCRIPT_PLAYERTYPE_GDI,
};
class SCRIPTS_API ScriptFactory {
private:
	ScriptFactory *mNext;
	const char *ScriptName;
	const char *ParamDescription;
public:
	virtual ~ScriptFactory();
	const char* GetName(void);
	const char* GetParamDescription(void);
	virtual ScriptImpClass* Create(void) = 0;
protected:
	friend class ScriptRegistrar;
	ScriptFactory(const char* name, const char* param);
	ScriptFactory* GetNext(void) const;
	void SetNext(ScriptFactory* link);
};

class SCRIPTS_API ScriptRegistrar {
private:
	static ScriptFactory *mScriptFactories;
public:
	static void RegisterScript(ScriptFactory* factory);
	static void UnregisterScript(ScriptFactory* factory);
	static ScriptImpClass* CreateScript(const char* name);
	static ScriptFactory* GetScriptFactory(const char* name);
	static ScriptFactory* GetScriptFactory(int index);
	static int Count(void);
};

template <class T> class ScriptRegistrant : public ScriptFactory {
public:
	ScriptRegistrant(const char* name, const char* param) : ScriptFactory(name, param)
	{
	}
	virtual ScriptImpClass* Create(void)
	{
		T* script = new T;
		script->SetFactory(this);
		script->Register_Auto_Save_Variables();
		return script;
	}
};

#define SCRIPT_COMMANDS_VERSION 174
struct ScriptCommands {
	unsigned int Size;
	unsigned int Version;
	void (*	Debug_Message )( char *format, ... );
	void ( * Action_Reset )( GameObject * obj, float priority );
	void ( * Action_Goto )( GameObject * obj, const ActionParamsStruct & params );
	void ( * Action_Attack )( GameObject * obj, const ActionParamsStruct & params );
	void ( * Action_Play_Animation )( GameObject * obj, const ActionParamsStruct & params );
	void ( * Action_Enter_Exit )( GameObject * obj, const ActionParamsStruct & params );
	void ( * Action_Face_Location )( GameObject * obj, const ActionParamsStruct & params );
	void ( * Action_Dock )( GameObject * obj, const ActionParamsStruct & params );
	void ( * Action_Follow_Input )( GameObject * obj, const ActionParamsStruct & params );
	void ( * Modify_Action )( GameObject * obj, int ActionID, const ActionParamsStruct & params, bool modify_move, bool modify_attack);
	int	( * Get_Action_ID )( GameObject * obj );
	bool	( * Get_Action_Params )( GameObject * obj, ActionParamsStruct & params );
	bool	( * Is_Performing_Pathfind_Action )( GameObject * obj );
	void ( * Set_Position )( GameObject * obj, const Vector3 & position );
	Vector3 ( * Get_Position )( GameObject * obj );
	Vector3 ( * Get_Bone_Position )( GameObject * obj, const char * bone_name );
	float ( * Get_Facing )( GameObject * obj );
	void ( * Set_Facing )( GameObject * obj, float degrees );
	void	( * Disable_All_Collisions )( GameObject * obj );
	void	( * Disable_Physical_Collisions )( GameObject * obj );
	void	( * Enable_Collisions )( GameObject * obj );
	void ( * Destroy_Object )( GameObject * obj );
	GameObject * ( * Find_Object )( int obj_id );
	GameObject * ( * Create_Object )( const char * type_name, const Vector3 & position );
	GameObject * ( * Create_Object_At_Bone )( GameObject * host_obj, const char * new_obj_type_name, const char * bone_name );
	int	( * Get_ID )( GameObject * obj );
	int	( * Get_Preset_ID )( GameObject * obj );
	const char * ( * Get_Preset_Name )( GameObject * obj );
	void (*Attach_Script)(GameObject* object, const char* scriptName, const char* scriptParams);
	void (*Add_To_Dirty_Cull_List)(GameObject* object);
	void	( * Start_Timer )( GameObject * obj, ScriptClass * script, float duration, int timer_id );
	void	( * Trigger_Weapon )( GameObject * obj, bool trigger, const Vector3 & target, bool primary);
	void	( * Select_Weapon )( GameObject * obj, const char * weapon_name );
	void	( * Send_Custom_Event )( GameObject * from, GameObject * to, int type, int param, float delay);
	void	( * Send_Damaged_Event )( GameObject * object, GameObject * damager );
	float	( * Get_Random )( float min, float max );
	int	( * Get_Random_Int )( int min, int max ); 
	GameObject * ( * Find_Random_Simple_Object )( const char *preset_name );
	void	( * Set_Model )( GameObject * obj, const char * model_name );

  /*!
  * Plays an animation upon the target game object, or a sub-object of that game object
  *
  * \param obj
  *   Game object upon which to play the animation, this must be a PhysicalGameObj
  * \param anim_name
  *   Name of the animation to play upon the game object, in the form model.animation
  * \param looping
  *   Set to true to make the entire animation play repeatedly in an infinite loop from start to
  *   end. Cannot be used in conjunction with the start_frame and end_frame parameters
  * \param sub_obj_name
  *   Not really sure what this is for... to be documented later :)
  * \param start_frame
  *   First frame number from the animation to be played
  * \param end_frame
  *   Last frame number from the animation to be played
  * \param is_blended
  *   Set to true to blend the animation with the current bone positions of the game object, this
  *   only works on instances of SoldierGameObj.
  *
  * \note
  *   Whilst this function does not allow you to create a looping animation from a subset of the
  *   frames within the overall animation you can still do this by restarting the animation when
  *   ScriptImpClass::Animation_Complete() is called. Alternatively you could use the new
  *   LoopedAnimationController class to handle this for you, leaving your script code free of
  *   the extra logic required to implement this functionality.
  */
	void	( * Set_Animation )( GameObject * obj, const char * anim_name, bool looping, const char * sub_obj_name, float start_frame, float end_frame, bool is_blended);
	void	( * Set_Animation_Frame )( GameObject * obj, const char * anim_name, int frame );
	int	( * Create_Sound )( const char * sound_preset_name, const Vector3 & position, GameObject * creator );
	int	( * Create_2D_Sound )( const char * sound_preset_name );
	int	( * Create_2D_WAV_Sound )( const char * wav_filename );
	int	( * Create_3D_WAV_Sound_At_Bone )( const char * wav_filename, GameObject * obj, const char * bone_name );
	int	( * Create_3D_Sound_At_Bone )( const char * sound_preset_name, GameObject * obj, const char * bone_name );
	int	( * Create_Logical_Sound )( GameObject * creator, int type, const Vector3 & position, float radius );	
	void	( * Start_Sound )( int sound_id );
	void	( * Stop_Sound )( int sound_id, bool destroy_sound);
	void	( * Monitor_Sound )( GameObject * game_obj, int sound_id );
	void	( * Set_Background_Music )( const char * wav_filename );
	void	( * Fade_Background_Music )( const char * wav_filename, int fade_out_time, int fade_in_time );
	void	( * Stop_Background_Music )( void );
	float	( * Get_Health )( GameObject * obj );
	float	( * Get_Max_Health )( GameObject * obj );
	void	( * Set_Health )( GameObject * obj, float health );
	float	( * Get_Shield_Strength )( GameObject * obj );
	float	( * Get_Max_Shield_Strength )( GameObject * obj );
	void	( * Set_Shield_Strength )( GameObject * obj, float strength );
	void	( * Set_Shield_Type )( GameObject * obj, const char * name );
	int	( * Get_Player_Type )( GameObject * obj );
	void	( * Set_Player_Type )( GameObject * obj, int type );
	float	( * Get_Distance )( const Vector3 & p1, const Vector3 & p2 );
	void	( * Set_Camera_Host )( GameObject * obj );
	void	( * Force_Camera_Look )( const Vector3 & target );
	GameObject * ( * Get_The_Star )( void );
	GameObject * ( * Get_A_Star )( const Vector3 & pos );
	GameObject * ( * Find_Closest_Soldier )( const Vector3 & pos, float min_dist, float max_dist, bool only_human);
	bool		    ( * Is_A_Star )( GameObject * obj );
	void	( * Control_Enable )( GameObject * obj, bool enable );
	const char * ( * Get_Damage_Bone_Name )( void );
	bool			 ( * Get_Damage_Bone_Direction )( void );
	bool	( * Is_Object_Visible)( GameObject * looker, GameObject * obj );
	void	( * Enable_Enemy_Seen)( GameObject * obj, bool enable);
	void	(*	Set_Display_Color )( unsigned char red, unsigned char green, unsigned char blue);
	void	(*	Display_Text )( int string_id );
	void	(*	Display_Float )( float value, const char * format);
	void	(*	Display_Int )( int value, const char * format);
	void	(*	Save_Data )( ScriptSaver & saver, int id, int size, void * data );
	void	(*	Save_Pointer )( ScriptSaver & saver, int id, void * pointer );
	bool	(*	Load_Begin )( ScriptLoader & loader, int * id );
	void	(*	Load_Data )( ScriptLoader & loader, int size, void * data );
	void	(*	Load_Pointer )( ScriptLoader & loader, void ** pointer );
	void	(*	Load_End )( ScriptLoader & loader );
	void (*Begin_Chunk)(ScriptSaver& saver, unsigned int chunkID);
	void (*End_Chunk)(ScriptSaver& saver);
	bool (*Open_Chunk)(ScriptLoader& loader, unsigned int* chunkID);
	void (*Close_Chunk)(ScriptLoader& loader);
	void	(* Clear_Radar_Markers )( void );
	void	(* Clear_Radar_Marker )( int id );
	void	(* Add_Radar_Marker )( int id, const Vector3& position, int shape_type, int color_type );
	void	(* Set_Obj_Radar_Blip_Shape )( GameObject * obj, int shape_type );
	void	(* Set_Obj_Radar_Blip_Color )( GameObject * obj, int color_type );
	void	(* Enable_Radar )( bool enable );
	void	(* Clear_Map_Cell )( int cell_x, int cell_y );	
	void	(* Clear_Map_Cell_By_Pos )( const Vector3 &world_space_pos );
	void	(* Clear_Map_Cell_By_Pixel_Pos )( int pixel_pos_x, int pixel_pos_y );
	void	(* Clear_Map_Region_By_Pos )( const Vector3 &world_space_pos, int pixel_radius );
	void	(* Reveal_Map )( void );
	void	(* Shroud_Map )( void );
	void	(* Show_Player_Map_Marker )( bool onoff );
	float	(* Get_Safe_Flight_Height )( float x_pos, float y_pos );
	void	(* Create_Explosion )( const char * explosion_def_name, const Vector3 & pos, GameObject * creator);
	void	(* Create_Explosion_At_Bone )( const char * explosion_def_name, GameObject * object, const char * bone_name, GameObject * creator);
	void	(* Enable_HUD )( bool enable );
	void	(* Mission_Complete )( bool success );
	void	(* Give_PowerUp )( GameObject * obj, const char * preset_name, bool display_on_hud);
	void (*Innate_Disable)(GameObject* object);
	void (*Innate_Enable)(GameObject* object);
	bool	(* Innate_Soldier_Enable_Enemy_Seen )( GameObject * obj, bool state );
	bool	(* Innate_Soldier_Enable_Gunshot_Heard )( GameObject * obj, bool state );
	bool	(* Innate_Soldier_Enable_Footsteps_Heard )( GameObject * obj, bool state );
	bool	(* Innate_Soldier_Enable_Bullet_Heard )( GameObject * obj, bool state );
	bool	(* Innate_Soldier_Enable_Actions )( GameObject * obj, bool state );
	void	(* Set_Innate_Soldier_Home_Location )( GameObject * obj, const Vector3& home_pos, float home_radius);
	void	(* Set_Innate_Aggressiveness )( GameObject * obj, float aggressiveness );
	void	(* Set_Innate_Take_Cover_Probability )( GameObject * obj, float probability );
	void	(* Set_Innate_Is_Stationary )( GameObject * obj, bool stationary );
	void	(* Innate_Force_State_Bullet_Heard )( GameObject * obj, const Vector3 & pos );
	void	(* Innate_Force_State_Footsteps_Heard )( GameObject * obj, const Vector3 & pos );
	void	(* Innate_Force_State_Gunshots_Heard )( GameObject * obj, const Vector3 & pos );
	void	(* Innate_Force_State_Enemy_Seen )( GameObject * obj, GameObject * enemy );
	void	(* Static_Anim_Phys_Goto_Frame )( int obj_id, float frame, const char * anim_name);
	void	(* Static_Anim_Phys_Goto_Last_Frame )( int obj_id, const char * anim_name);
	unsigned int (* Get_Sync_Time)( void );
	void	(* Add_Objective)( int id, int type, int status, int short_description_id, char * description_sound_filename, int long_description_id);
	void	(* Remove_Objective)( int id );
	void	(* Set_Objective_Status)( int id, int status );
	void	(* Change_Objective_Type)( int id, int type );
	void	(* Set_Objective_Radar_Blip)( int id, const Vector3 & position );
	void	(* Set_Objective_Radar_Blip_Object)( int id, ScriptableGameObj * unit );
	void	(* Set_Objective_HUD_Info)( int id, float priority, const char * texture_name, int message_id );
	void	(* Set_Objective_HUD_Info_Position)( int id, float priority, const char * texture_name, int message_id, const Vector3 & position );
	void	(* Shake_Camera)( const Vector3 & pos, float radius, float intensity, float duration);
	void	(* Enable_Spawner)( int id, bool enable );
	GameObject * (* Trigger_Spawner)( int id );
	void	(* Enable_Engine)( GameObject* object, bool onoff );
	int	(* Get_Difficulty_Level)( void );
	void	(* Grant_Key)( GameObject* object, int key, bool grant);
	bool	(* Has_Key)( GameObject* object, int key );
	void	(* Enable_Hibernation)( GameObject * object, bool enable );
	void	(* Attach_To_Object_Bone)( GameObject * object, GameObject * host_object, const char * bone_name );
	int	(* Create_Conversation)( const char *conversation_name, int priority, float max_dist, bool is_interruptable);
	void	(* Join_Conversation)( GameObject * object, int active_conversation_id, bool allow_move, bool allow_head_turn, bool allow_face);
	void	(* Join_Conversation_Facing)( GameObject * object, int active_conversation_id, int obj_id_to_face );
	void	(* Start_Conversation)( int active_conversation_id, int ActionID);
	void	(* Monitor_Conversation)( GameObject * object, int active_conversation_id );
	void	(* Start_Random_Conversation)( GameObject * object );
	void	(* Stop_Conversation)( int active_conversation_id );
	void	(* Stop_All_Conversations)( void );
	void	(* Lock_Soldier_Facing)( GameObject * object, GameObject * object_to_face, bool turn_body );
	void	(* Unlock_Soldier_Facing)( GameObject * object );
	void	(* Apply_Damage)( GameObject * object, float amount, const char * warhead_name, GameObject * damager);
	void	(* Set_Loiters_Allowed)( GameObject * object, bool allowed );
	void	(* Set_Is_Visible)( GameObject * object, bool visible );
	void	(* Set_Is_Rendered)( GameObject * object, bool rendered );
	float	(* Get_Points)( GameObject * object );
	void	(* Give_Points)( GameObject * object, float points, bool entire_team );
	float	(* Get_Money)( GameObject * object );
	void	(* Give_Money)( GameObject * object, float money, bool entire_team );
	bool	(* Get_Building_Power)( GameObject * object );
	void	(* Set_Building_Power)( GameObject * object, bool onoff );
	void	(* Play_Building_Announcement)( GameObject * object, int text_id );
	GameObject * (* Find_Nearest_Building_To_Pos )( const Vector3 & position, const char * mesh_prefix );
	GameObject * (* Find_Nearest_Building )( GameObject * object, const char * mesh_prefix );
	int	(* Team_Members_In_Zone)( GameObject * object, int player_type );
	void  (*Set_Clouds) (float cloudcover, float cloudgloominess, float ramptime);
	void  (*Set_Lightning) (float intensity, float startdistance, float enddistance, float heading, float distribution, float ramptime);
	void  (*Set_War_Blitz) (float intensity, float startdistance, float enddistance, float heading, float distribution, float ramptime);
	void	(*Set_Wind)			(float heading, float speed, float variability, float ramptime);
	void	(*Set_Rain)			(float density, float ramptime, bool prime);
	void	(*Set_Snow)			(float density, float ramptime, bool prime);
	void	(*Set_Ash)			(float density, float ramptime, bool prime);
	void  (*Set_Fog_Enable) (bool enabled);
	void  (*Set_Fog_Range)	(float startdistance, float enddistance, float ramptime);
	void	(*Enable_Stealth)	(GameObject * object, bool onoff);
	void	(*Cinematic_Sniper_Control)	(bool enabled, float zoom);
	int	(*Text_File_Open)			( const char * filename );
	bool	(*Text_File_Get_String)	( int handle, char * buffer, int size );
	void	(*Text_File_Close)		( int handle );
	void	(*Enable_Vehicle_Transitions)	( GameObject * object, bool enable );
	void	(*Display_GDI_Player_Terminal)		();
	void	(*Display_NOD_Player_Terminal)		();
	void	(*Display_Mutant_Player_Terminal)	();
	bool	(*Reveal_Encyclopedia_Character)	( int object_id );
	bool	(*Reveal_Encyclopedia_Weapon)		( int object_id );
	bool	(*Reveal_Encyclopedia_Vehicle)	( int object_id );
	bool	(*Reveal_Encyclopedia_Building)	( int object_id );
	void	(*Display_Encyclopedia_Event_UI) ( void );
	void	(* Scale_AI_Awareness)( float sight_scale, float hearing_scale );
	void	(* Enable_Cinematic_Freeze)( GameObject * object, bool enable );
	void	(* Expire_Powerup )( GameObject * object );
	void	(* Set_HUD_Help_Text )( int string_id, const Vector3 &color );
	void	(* Enable_HUD_Pokable_Indicator )( GameObject * object, bool enable );
	void	(* Enable_Innate_Conversations )( GameObject * object, bool enable );
	void	(* Display_Health_Bar )( GameObject * object, bool display );
	void	(* Enable_Shadow) ( GameObject * object, bool enable );
	void	(* Clear_Weapons) ( GameObject * object );
	void	(* Set_Num_Tertiary_Objectives) ( int count );
	void	(* Enable_Letterbox) ( bool onoff, float seconds );
	void	(* Set_Screen_Fade_Color) ( float r, float g, float b, float seconds );
	void	(* Set_Screen_Fade_Opacity) ( float opacity, float seconds );
};

class ScriptCommandsClass {
public:
	ScriptCommands	*Commands;
};
extern SCRIPTS_API ScriptCommands* Commands;
#endif
