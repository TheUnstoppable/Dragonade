/*	Renegade Scripts.dll
    Dragonade Game Feature Class
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAGAMEFEATURE
#define INCLUDE_DAGAMEFEATURE

class DAGameFeatureClass;

class DAGameFeatureFactoryClass abstract {
public:
	DA_API DAGameFeatureFactoryClass(const char *Name,const char *EnableSetting,const char *RequiredData = 0);
	virtual ~DAGameFeatureFactoryClass() { }
	inline const char *Get_Name() const {
		return Name;
	}
	inline const char *Get_Enable_Setting() const {
		return EnableSetting;
	}
	inline const char *Get_Required_Data() const {
		return RequiredData;
	}
	inline DAGameFeatureClass *Get_Instance() const {
		return Instance;
	}
	bool Check_Map_Support() const;
	bool Check_Enabled() const;
	virtual DAGameFeatureClass *Create_Instance() = 0;
	void Destroy_Instance();
	
protected:
	DA_API _declspec(deprecated) void Add_Game_Feature();
	const char *Name;
	const char *EnableSetting;
	const char *RequiredData;
	DAGameFeatureClass *Instance;
};

class DA_API DAGameFeatureClass abstract {
public:
	inline const char *Get_Name() {
		return Factory->Get_Name();
	}
	inline const DAGameFeatureFactoryClass *Get_Factory() {
		return Factory;
	}
	inline void Set_Factory(const DAGameFeatureFactoryClass *Factory) {
		this->Factory = Factory;
	}
	virtual void Init() = 0;
	virtual ~DAGameFeatureClass() { }
	
protected:
	const DAGameFeatureFactoryClass *Factory;
};

template <class T> class DAGameFeatureRegistrant : public DAGameFeatureFactoryClass {
public:
	DAGameFeatureRegistrant(const char *Name,const char *EnableSetting,const char *RequiredData = 0) : DAGameFeatureFactoryClass(Name,EnableSetting,RequiredData) { }
	virtual DAGameFeatureClass *Create_Instance() {
		if (!Instance) {
			Instance = new T;
			Instance->Set_Factory(this);
			Instance->Init();
		}
		return Instance;
	}
};

#define Register_Game_Feature(ClassName,Name,EnableSetting,RequiredData) DAGameFeatureRegistrant<ClassName> ClassName##Registrant(Name,EnableSetting,RequiredData);

#endif
