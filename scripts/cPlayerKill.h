#ifndef TT_INCLUDE_CPLAYERKILL_H
#define TT_INCLUDE_CPLAYERKILL_H
#include "cNetEvent.h"

class cPlayerKill : public cNetEvent {
public:
	int id1;
	int id2;
	cPlayerKill();
	void Init(int player1, int player2);
	virtual uint Get_Network_Class_ID() const;
	virtual void Import_Creation(BitStreamClass& stream);
	virtual void Export_Creation(BitStreamClass& stream);
	virtual void Act();
};

#endif