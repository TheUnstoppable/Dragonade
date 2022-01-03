/*	Renegade Scripts.dll
    Dragonade Game Mode Class
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAGAMEMODE
#define INCLUDE_DAGAMEMODE

class DAGameModeClass;

class DAGameModeFactoryClass abstract {
public:
	DA_API DAGameModeFactoryClass(const char *ShortName,const char *LongName,const char *RequiredData = 0);
	virtual ~DAGameModeFactoryClass() { }
	inline const char *Get_Long_Name() const {
		return LongName;
	}
	inline const char *Get_Short_Name() const {
		return ShortName;
	}
	inline DAGameModeClass *Get_Instance() const {
		return Instance;
	}
	bool Check_Map_Support() const;
	virtual DAGameModeClass *Create_Instance() = 0;
	void Destroy_Instance();

protected:
	DA_API _declspec(deprecated) void Add_Game_Mode();
	const char *ShortName;
	const char *LongName;
	const char *RequiredData;
	DAGameModeClass *Instance;
};

class DAGameModeClass abstract {
public:
	inline const char *Get_Long_Name() const {
		return Factory->Get_Long_Name();
	}
	inline const char *Get_Short_Name() const {
		return Factory->Get_Short_Name();
	}
	inline const DAGameModeFactoryClass *Get_Factory() {
		return Factory;
	}
	inline void Set_Factory(const DAGameModeFactoryClass *Factory) {
		this->Factory = Factory;
	}
	virtual void Init() = 0;
	virtual ~DAGameModeClass() { }

protected:
	const DAGameModeFactoryClass *Factory;
};

template <typename T> class DAGameModeRegistrant : public DAGameModeFactoryClass {
public:
	DAGameModeRegistrant(const char *ShortName,const char *LongName,const char *RequiredData = 0) : DAGameModeFactoryClass(ShortName,LongName,RequiredData) { }
	virtual DAGameModeClass *Create_Instance() {
		if (!Instance) {
			Instance = new T;
			Instance->Set_Factory(this);
			Instance->Init();
		}
		return Instance;
	}
};

class DANullGameModeClass : public DAGameModeClass {
	virtual void Init() { }
};

class DANullGameModeRegistrant : public DAGameModeRegistrant<DANullGameModeClass> {
public:
	DANullGameModeRegistrant(const char *ShortName,const char *LongName) : DAGameModeRegistrant<DANullGameModeClass>(ShortName,LongName,0) { }
};

#define Register_Game_Mode(ClassName,ShortName,LongName,RequiredData) DAGameModeRegistrant<ClassName> ClassName##Registrant(ShortName,LongName,RequiredData);
#define Register_Null_Game_Mode(ShortName,LongName) static DANullGameModeRegistrant NullGameModeRegistrant(ShortName,LongName);

#endif