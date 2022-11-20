#ifndef TT_INCLUDE__RIGIDBODYCLASS_H
#define TT_INCLUDE__RIGIDBODYCLASS_H
#include "MoveablePhysClass.h"
#include "ODESystemClass.h"
#include "AABoxClass.h"
#include "Quaternion.h"
#include "Matrix3.h"
#include "PhysicsConstants.h"
class RigidBodyDefClass;
class OBBoxRenderObjClass;
class PaddedBoxClass;
class OctBoxClass;
class RBodyHistoryClass;
struct CastResultStruct;
const int MAX_CONTACT_POINTS = 10;
const float RBODY_SLEEP_DELAY	= 0.5f;	// if still for this many seconds, shut off simulation
struct RigidBodyStateStruct 
{
	RigidBodyStateStruct(void) {}
	RigidBodyStateStruct(const RigidBodyStateStruct & that);
	RigidBodyStateStruct & operator = (const RigidBodyStateStruct & that);
	void				Reset(void);
	void				To_Vector(StateVectorClass & vec);
	int				From_Vector(const StateVectorClass & vec,int index);
	static void		Lerp(const RigidBodyStateStruct & s0,const RigidBodyStateStruct & s1,float fraction,RigidBodyStateStruct * res);
	bool				Is_Valid(void) { return (Position.Is_Valid() && Orientation.Is_Valid() && LMomentum.Is_Valid() && AMomentum.Is_Valid()); }
	Vector3			Position;
	Quaternion		Orientation;
	Vector3			LMomentum;
	Vector3			AMomentum;
};
inline RigidBodyStateStruct::RigidBodyStateStruct(const RigidBodyStateStruct & that)
{
	*this = that;
}
inline RigidBodyStateStruct & RigidBodyStateStruct::operator = (const RigidBodyStateStruct & that)
{
	Position = that.Position;
	Orientation = that.Orientation;
	LMomentum = that.LMomentum;
	AMomentum = that.AMomentum;
	return *this;
}
inline void RigidBodyStateStruct::Reset(void)
{
	Position.Set(0,0,0);
	Orientation.Make_Identity();
	LMomentum.Set(0,0,0);
	AMomentum.Set(0,0,0);
}
inline void RigidBodyStateStruct::To_Vector(StateVectorClass & vec)
{
	vec.Add(Position[0]);
	vec.Add(Position[1]);
	vec.Add(Position[2]);
	
	vec.Add(Orientation[0]);
	vec.Add(Orientation[1]);
	vec.Add(Orientation[2]);
	vec.Add(Orientation[3]);

	vec.Add(LMomentum[0]);
	vec.Add(LMomentum[1]);
	vec.Add(LMomentum[2]);

	vec.Add(AMomentum[0]);
	vec.Add(AMomentum[1]);
	vec.Add(AMomentum[2]);
}
inline int RigidBodyStateStruct::From_Vector(const StateVectorClass & vec,int index)
{
	Position[0] = vec[index++];
	Position[1] = vec[index++];
	Position[2] = vec[index++];

	Orientation[0] = vec[index++];
	Orientation[1] = vec[index++];
	Orientation[2] = vec[index++];
	Orientation[3] = vec[index++];

	LMomentum[0] = vec[index++];
	LMomentum[1] = vec[index++];
	LMomentum[2] = vec[index++];
	
	AMomentum[0] = vec[index++];
	AMomentum[1] = vec[index++];
	AMomentum[2] = vec[index++];

	return index;
}
inline void RigidBodyStateStruct::Lerp
(
	const RigidBodyStateStruct & s0,
	const RigidBodyStateStruct & s1,
	float fraction,
	RigidBodyStateStruct * res
)
{
	Vector3::Lerp(s0.Position,s1.Position,fraction,&(res->Position));
	Vector3::Lerp(s0.LMomentum,s1.LMomentum,fraction,&(res->LMomentum));
	Vector3::Lerp(s0.AMomentum,s1.AMomentum,fraction,&(res->AMomentum));
	::Fast_Slerp(res->Orientation,s0.Orientation,s1.Orientation,fraction);
}

class RigidBodyClass : public MoveablePhysClass , public ODESystemClass
{
public:
	RigidBodyClass(void);
	virtual ~RigidBodyClass(void);
	virtual RigidBodyClass *	As_RigidBodyClass(void) { return this; }
	RigidBodyDefClass *			Get_RigidBodyDef(void) { return (RigidBodyDefClass *)Definition; }
	void								Init(const RigidBodyDefClass & definition);
	virtual const AABoxClass & Get_Bounding_Box(void) const;
	virtual const Matrix3D &	Get_Transform(void) const;
	virtual void					Set_Transform(const Matrix3D & m);
	virtual bool					Cast_Ray(PhysRayCollisionTestClass & raytest);
	virtual bool					Cast_AABox(PhysAABoxCollisionTestClass & boxtest);
	virtual bool					Cast_OBBox(PhysOBBoxCollisionTestClass & boxtest);
	virtual bool					Intersection_Test(PhysAABoxIntersectionTestClass & test);
	virtual bool					Intersection_Test(PhysOBBoxIntersectionTestClass & test);
	virtual bool					Intersection_Test(PhysMeshIntersectionTestClass & test);
	virtual bool					Can_Teleport(const Matrix3D &new_tm, bool check_dyn_only = false,MultiListClass<PhysClass> * result_list = NULL);
	virtual bool					Can_Teleport_And_Stand(const Matrix3D &new_tm, Matrix3D *out);
	void								Get_Orientation(Quaternion * set_q) const { *set_q = State.Orientation; }
	void								Set_Orientation(Quaternion & new_q) { State.Orientation = new_q; }
	virtual void					Get_Velocity(Vector3 * set_vel) const;
	virtual void					Get_Angular_Velocity(Vector3 * set_avel) const;
	virtual void					Set_Velocity(const Vector3 & newvel);
	virtual void					Set_Angular_Velocity(const Vector3 & newavel);
	virtual void					Apply_Impulse(const Vector3 & imp);
	virtual void					Apply_Impulse(const Vector3 & imp, const Vector3 & wpos);
	virtual void					Timestep(float dt); 
	void								Compute_Point_Velocity(const Vector3 & p,Vector3 * pdot);
	float								Get_Last_Timestep(void)		{ return LastTimestep; }
	void								Network_Interpolate_State_Update(	const Vector3 & new_pos,
																						const Quaternion & new_orientation,
																						const Vector3 & new_vel,
																						const Vector3 & new_avel,
																						float blend_fraction);
	void								Network_Latency_State_Update(			const Vector3 & new_pos,
																						const Quaternion & new_orientation,
																						const Vector3 & new_vel,
																						const Vector3 & new_avel);
	static void						Set_Correction_Time(float time)	{ _CorrectionTime = time; }
	static float					Get_Correction_Time(void)			{ return _CorrectionTime; }
	virtual void					Set_Model(RenderObjClass * model);
	virtual void					Set_Mass(float mass);
	virtual void					Get_Inertia_Inv(Matrix3 * set_I_inv);
	void								Set_Inertia(const Matrix3 & I);
	void								Get_Inertia(Matrix3 * I);
	void								Set_Contact_Parameters(float length);
	void								Get_Contact_Parameters(float * stiffness,float * damping,float * length);
	float								Get_Weight(void) { return GravScale * Mass * (-PhysicsConstants::GravityAcceleration.Z); }
	virtual void					Get_Shadow_Blob_Box(AABoxClass * set_obj_space_box);
	virtual bool					Push(const Vector3 & move);
	virtual const PersistFactoryClass &	Get_Factory (void) const;
	virtual bool								Save (ChunkSaveClass &csave);
	virtual bool								Load (ChunkLoadClass &cload);
	virtual void								On_Post_Load(void);	
	OBBoxRenderObjClass*			Peek_Box() { return Box; }
protected:
	virtual void					Get_State(StateVectorClass & set_state);
	virtual int						Set_State(const StateVectorClass & new_state,int start_index);
	void								Set_State(const RigidBodyStateStruct & new_state);
	virtual int						Compute_Derivatives(float t,StateVectorClass * test_state,StateVectorClass * set_derivs,int start_index);
	void								Integrate(float time);
	void								Update_Cached_Model_Parameters(void);
	virtual void					Compute_Inertia(void);
	void								Update_Auxiliary_State(void);
	virtual void					Compute_Force_And_Torque(Vector3 * force,Vector3 * torque);
	void								Compute_Impact(const CastResultStruct & result,Vector3 * impulse);
	void								Compute_Impact(const Vector3 & point,const Vector3 & normal,Vector3 * impulse);
	bool								Is_Colliding(const Vector3 & point, const Vector3 & normal);
	void								Set_Moving_Collision_Region(float dt);
	void								Set_Stationary_Collision_Region(void);
	virtual bool					Can_Go_To_Sleep(float dt);
	bool								Find_State_In_Contact_Zone(const RigidBodyStateStruct & oldstate);
	bool								Push_Phys3_Object_Away(Phys3Class * p3obj,const CastResultStruct & contact);
	void								Network_Latency_Error_Correction(float dt);
	void								Assert_State_Valid(void) const;
	void								Assert_Not_Intersecting(void);
	void								Dump_State(void) const;
	OBBoxRenderObjClass *		Box;					// World-Space Box
	Matrix3							IBody;				// Body space inertia tensor
	Matrix3							IBodyInv;			// Inverse of the body space inertia tensor
	OctBoxClass *					ContactBox;			// datastructure for finding contact points
	float								ContactThickness;	// Dimension of the contact region
	RigidBodyStateStruct			State;				// current state vector
	Matrix3							Rotation;
	Matrix3							IInv;		
	Vector3							Velocity;
	Vector3							AngularVelocity;
	bool								IsInContact;
	int								ContactCount;
	Vector3							ContactPoint[MAX_CONTACT_POINTS];
	Vector3							ContactNormal;
	int								StickCount;
	float								LastTimestep;
	float								GoToSleepTimer;
	RBodyHistoryClass	*			History;
	RigidBodyStateStruct			LatencyError;
	RigidBodyStateStruct			LastKnownState;
	static float					_CorrectionTime;
private:
	RigidBodyClass(const RigidBodyClass &);
	RigidBodyClass & operator = (const RigidBodyClass &);
}; // 02B0  02B0  02CC  02C8
#endif
