#ifndef TT_INCLUDE__PHYSICSCONSTANTS_H
#define TT_INCLUDE__PHYSICSCONSTANTS_H



#include "Vector3.h"



class ChunkSaveClass;
class ChunkLoadClass;


#ifndef TTLE_EXPORTS
extern float ContactFrictionCoefficient[3][32];
extern float ContactDragCoefficient[3][32];
#else
typedef float ContactFrictionCoefficientType[3][32]; 
extern REF_DECL(ContactFrictionCoefficientType,ContactFrictionCoefficient); 
typedef float ContactDragCoefficientType[3][32];
extern REF_DECL(ContactDragCoefficientType,ContactDragCoefficient);
#endif


class PhysicsConstants
{

public:

	static float AngularDamping;
	static float DefaultContactFriction;
	static float DefaultContactDrag;
#ifndef W3DVIEWER
	static REF_DECL(Vector3, GravityAcceleration);
#endif
	static float LinearDamping;
	static float RestingContactVelocity;
	static float MinFrictionVelocity;
	static float MinFrictionVelocity2;

	static void Init();
	
	static void Set_Contact_Friction_Coefficient(int physicsType, int surfaceType, float value)
	{
		if ((uint)physicsType < 3 && (uint)surfaceType < 32)
			ContactFrictionCoefficient[physicsType][surfaceType] = value;
	}
	
	static float Get_Contact_Friction_Coefficient(int physicsType, int surfaceType)
	{
		if ((uint)physicsType < 3 && (uint)surfaceType < 32)
			return ContactFrictionCoefficient[physicsType][surfaceType];
		else
			return ContactFrictionCoefficient[0][0];
	}

	static void Set_Contact_Drag_Coefficient(int physicsType, int surfaceType, float value)
	{
		if ((uint)physicsType < 3 && (uint)surfaceType < 32)
			ContactDragCoefficient[physicsType][surfaceType] = value;
	}

	static float Get_Contact_Drag_Coefficient(int physicsType, int surfaceType)
	{
		if ((uint)physicsType < 3 && (uint)surfaceType < 32)
			return ContactDragCoefficient[physicsType][surfaceType];
		else
			return ContactDragCoefficient[0][0];
	}

	static void Save(ChunkSaveClass&);
	static void Load(ChunkLoadClass&);

};



#endif
