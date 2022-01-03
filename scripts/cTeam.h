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
#ifndef TT_INCLUDE__CTEAM_H
#define TT_INCLUDE__CTEAM_H



#include "NetworkObjectClass.h"
#include "engine_string.h"



#define MAX_TEAMS 2



class cTeam :
	public NetworkObjectClass
{

private:

	WideStringClass name; // 06B4
	int kills; // 06B8
	int deaths; // 06BC
	float score; // 06C0
	int unk06C4; // 06C4; Not sure about type, but this variable never seems to be used.
	int id; // 06C8

public:

	cTeam();
	~cTeam();
	void Init(int _id);
	void Init_Team_Name();
	void Reset();
	SCRIPTS_API void Set_Kills(int _kills); //DA
	SCRIPTS_API void Decrement_Kills(); //DA
	SCRIPTS_API void Decrement_Deaths(); //DA
	SCRIPTS_API void Set_Deaths(int _deaths); //DA
	SCRIPTS_API void Set_Score(float _score);
	int Tally_Size();
	SCRIPTS_API void Increment_Kills(); //DA
	SCRIPTS_API void Increment_Deaths(); //DA
	SCRIPTS_API void Increment_Score(float amount); //DA
	SCRIPTS_API float Get_Kill_To_Death_Ratio(); //DA
	void Get_Team_String(int, WideStringClass&);
	Vector3 Get_Color();
	int Tally_Money();
	void Export_Creation(BitStreamClass& bitStream);
	void Import_Creation(BitStreamClass& bitStream);
	void Export_Rare(BitStreamClass& bitStream);
	void Import_Rare(BitStreamClass& bitStream);
	void Export_Occasional(BitStreamClass& bitStream);
	void Import_Occasional(BitStreamClass& bitStream);
	void Export_Frequent(BitStreamClass& bitStream) {}
	void Import_Frequent(BitStreamClass& bitStream) {}

	unsigned int Get_Network_Class_ID() const;
	void Delete() { delete this; }
	WideStringClass Get_Name() const { return name; }
	int Get_Id() const { return id; }
	int Get_Kills() const { return kills; }
	int Get_Deaths() const { return deaths; }
	float Get_Score() const { return score; }

}; // 06CC



#endif