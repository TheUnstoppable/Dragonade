


class DB_Shield_Generator_Vehicle : public ScriptImpClass {
	unsigned long shroudID;
	unsigned long sizeID;
	int PilotID;
	bool CoolDown;
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int type, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Destroyed(GameObject *obj);
};


class DB_Controllable_Turret : public ScriptImpClass {
	int Team;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int Number);
};

class DB_Guard_Bot : public ScriptImpClass {
	Vector3 HomePosition;
	bool OnLadder;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class DB_Support_Bot : public ScriptImpClass {
	int followid;
	float distance;
	float speed;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class dblaney_Nod_Obelisk : public ScriptImpClass {
	int WeaponID;
	bool PowerOn;
	float OffsetHeight;
	float MaxRange;
	float ChargeTime;
	int GroundOnly;
	void Created(GameObject* ObeliskObj);
	void Killed(GameObject* ObeliskObj, GameObject* Killer);
	void Destroyed(GameObject* ObeliskObj);
	void Timer_Expired(GameObject *ObeliskObj, int number);
	//void Custom(GameObject* ObeliskObj, int type, int Param, GameObject* Sender);
};

class dblaney_Nod_Obelisk_Structure : public ScriptImpClass {
	int WeaponID;
	bool PowerOn;
	float OffsetHeight;
	float MaxRange;
	float ChargeTime;
	int GroundOnly;
	void Created(GameObject* ObeliskObj);
	void Killed(GameObject* ObeliskObj, GameObject* Killer);
	void Destroyed(GameObject* ObeliskObj);
	void Custom(GameObject* ObeliskObj, int type, int Param, GameObject* Sender);
};

class dblaney_Nod_Obelisk_Weapon : public ScriptImpClass {
	int EnemyID;
	int EffectID;
	bool Firing;
	bool Charged;
	float MaxRange;
	float ChargeTime;
	int GroundOnly;

	void Created(GameObject* WeaponObj);
	void Destroyed(GameObject* WeaponObj);
	bool IsValidEnemy(GameObject* WeaponObj, GameObject* EnemyObj);

	void StartFiring(GameObject* WeaponObj);
	void StopFiring(GameObject* WeaponObj);
	void StartEffect(GameObject* WeaponObj);
	void StopEffect(GameObject* WeaponObj);
	void FireAt(GameObject* WeaponObj, GameObject* EnemyObj);
	void FaceEnemy(GameObject* WeaponObj, GameObject* EnemyObj);
	void StopFireAt(GameObject* WeaponObj);
	void Timer_Expired(GameObject* WeaponObj, int Number);
	void Enemy_Seen(GameObject* WeaponObj, GameObject* EnemyObj);
public: void Register_Auto_Save_Variables();
};


class DB_Power_Down_Console : public ScriptImpClass {
	bool reset;
	int team;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class DB_Power_Down_Building_Console : public ScriptImpClass {
	bool reset;
	int team;
	int buildingID;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class DB_Disable_Injured_Animations : public ScriptImpClass {
	void Created(GameObject *obj);
	void Detach(GameObject *obj);
};

class DB_Set_Scale : public ScriptImpClass {
	float originalscale;
	void Created(GameObject *obj);
	void Detach(GameObject *obj);
};

class DB_Grant_Keycard_Killed : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *killer);
	void Destroyed(GameObject *obj);
};

class DB_Grant_Keycard_Timer : public ScriptImpClass {
	int KeyLevel;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class DB_Infantry_Place_Buildable_Object : public ScriptImpClass {
	bool canRegen;
	int previewObjectId;
	int placementBlocked;
	int reloadTime;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
	void DestroyPreview();
public:
	DB_Infantry_Place_Buildable_Object()
	{
		placementBlocked = 0;
	}
};

class DB_Stationary_Stealth : public ScriptImpClass {
	float xpos;
	float ypos;
	float zpos;
	bool isHidden;
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj,int number);
};

class DB_Turret_Sound : public ScriptImpClass
{
protected:
  void Created ( GameObject* obj );
  void Timer_Expired ( GameObject* obj, int number );
  float Get_Turret_Facing ( class RenderObjClass* obj );
  void Play_Sound ( GameObject* obj );
  void Stop_Sound ( GameObject* obj );

  float lastFacing;
  int emitterID;

  float minRotate;
};

class DB_Turret_Spawn : public ScriptImpClass
{
  int turretId;
  bool hasDriver;
  bool bombard;
  void Created(GameObject *obj);
  void Custom(GameObject *obj,int message,int param,GameObject *sender);
  void Killed(GameObject *obj,GameObject *killer);
  void Destroyed(GameObject *obj);
};

class DB_Spawned_Visible_Ammo : public ScriptImpClass {
	ReferencerClass Ammo;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
};

class DB_Vehicle_Visible_Ammo : public ScriptImpClass {
	int enabled;
	int messageOn;
	int messageOff;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class DB_Orca_Lifter : public ScriptImpClass {
	int zoneID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
};

class DB_Turret_Bombard_Ability : public ScriptImpClass {
	int PilotID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class DB_Turret_Bombard_Player : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class DB_Health_Sound_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class DB_Visible_Passenger : public ScriptImpClass {
	int modelid;
	int passengerid;
	bool stealth;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
};

class DB_Drop_Wreckage_On_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};
class DB_Wreckage_Rebuildable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj, int number);
	bool firstdamage;
};

class DB_Supply_Truck : public ScriptImpClass
{
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class DB_Damage_Vehicle_Occupants_Area_Killed : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class DB_Damage_Vehicle_Occupants_Area_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class DB_Set_PT_Slot : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class DB_Spawn_When_Killed : public ScriptImpClass {
	void Killed (GameObject *obj,GameObject *killer);
};

class DB_Replace_When_Repaired : public ScriptImpClass {
	void Damaged (GameObject *obj,GameObject *damager,float amount);
};

class DB_Force_Fire : public ScriptImpClass {
	void Created(GameObject *obj);
	void Start_Attack(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Detach(GameObject *obj);
};