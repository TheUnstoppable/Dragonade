#ifndef TT_INCLUDE__ODESYSTEMCLASS_H
#define TT_INCLUDE__ODESYSTEMCLASS_H
#include "engine_vector.h"
class StateVectorClass : public DynamicVectorClass<float> 
{
public:
	void Reset(void) { ActiveCount = 0; }
	void Resize(int size) { if (size > VectorMax) { DynamicVectorClass<float>::Resize(size); } }
};
class ODESystemClass
{
public:
	virtual void	Get_State(StateVectorClass & set_state) = 0;
	virtual int		Set_State(const StateVectorClass & new_state,int start_index = 0) = 0;
	virtual int		Compute_Derivatives(float t,StateVectorClass * test_state,StateVectorClass * dydt,int start_index = 0) = 0;
};
class IntegrationSystem
{
public:
	static void Euler_Integrate(ODESystemClass * sys,float dt);
	static void	Midpoint_Integrate(ODESystemClass * sys,float dt);
	static void	Runge_Kutta_Integrate(ODESystemClass * sys,float dt);
	static void Runge_Kutta5_Integrate(ODESystemClass * odesys,float dt);
};

#endif
