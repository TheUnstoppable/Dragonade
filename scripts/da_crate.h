/*	Renegade Scripts.dll
    Dragonade Crate Classes
	Copyright 2017 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DACRATE
#define INCLUDE_DACRATE

namespace DACrateModifierOperator {
	enum Operator {
		NONE,
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE
	};
};

class DACrateModifierClass;

class DACrateModifierFactoryClass abstract {
public:
	DA_API DACrateModifierFactoryClass(const char *Name);
	virtual ~DACrateModifierFactoryClass() { }
	virtual DACrateModifierClass *Create(const char *Parameters) = 0;
	inline const char *Get_Name() const {
		return Name;
	}

protected:
	const char *Name;
};

class DA_API DACrateModifierClass abstract {
public:
	virtual void Init(const char *Parameters);
	virtual void Calculate_Odds(float &Odds,cPlayer *Player) = 0;
	void Apply_Modifier(float &Odds);
	bool Is_Multiplicative();
	bool Is_Positive();
	virtual ~DACrateModifierClass() { }
	inline void Set_Factory(const DACrateModifierFactoryClass *Factory) {
		this->Factory = Factory;
	}
	inline const DACrateModifierFactoryClass *Get_Factory() {
		return Factory;
	}
	inline const char *Get_Name() const {
		return Factory->Get_Name();
	}

protected:
	DACrateModifierOperator::Operator Operator;
	float Value;
	const DACrateModifierFactoryClass *Factory;
};

template <typename T> class DACrateModifierRegistrant : public DACrateModifierFactoryClass {
public:
	DACrateModifierRegistrant(const char *Name) : DACrateModifierFactoryClass(Name) { }
	virtual DACrateModifierClass *Create(const char *Parameters) {
		DACrateModifierClass *Mod = new T;
		Mod->Set_Factory(this);
		Mod->Init(Parameters);
		return Mod;
	}
};

#define Register_Crate_Modifier(ClassName,Name) DACrateModifierRegistrant<ClassName> ClassName##Registrant(Name);



namespace DACrateType {
	enum Type {
		INFANTRY = 0x1,
		VEHICLE = 0x2
	};
};
inline DACrateType::Type operator|(DACrateType::Type First,DACrateType::Type Second) {
	return (DACrateType::Type)(+First | +Second);
}
inline DACrateType::Type operator|=(DACrateType::Type &First,DACrateType::Type Second) {
	return First = (DACrateType::Type)(+First | +Second);
}
inline DACrateType::Type operator&=(DACrateType::Type &First,DACrateType::Type Second) {
	return First = (DACrateType::Type)(+First & +Second);
}
inline DACrateType::Type operator~(DACrateType::Type First) {
	return (DACrateType::Type)~(+First);
}

class DACrateClass;

class DACrateFactoryClass abstract {
public:
	DA_API DACrateFactoryClass(const char *Name,DACrateType::Type Type);
	virtual ~DACrateFactoryClass() { }
	virtual DACrateClass *Create_Instance() = 0;
	DA_API void Destroy_Instance();
	DA_API bool Check_Enabled() const;
	DA_API bool Check_Type(DACrateType::Type Type) const;
	DA_API bool Check_Enabled_Type(DACrateType::Type Type) const;
	inline const char *Get_Name() const {
		return Name;
	}
	inline const char *Get_Section() const {
		return Section;
	}
	inline DACrateType::Type Get_Type() const {
		return Type;
	}
	DACrateType::Type Get_Enabled_Type() const;
	inline DACrateClass *Get_Instance() const {
		return Instance;
	}
	
protected:
	const char *Name;
	StringClass Section;
	DACrateType::Type Type;
	DACrateClass *Instance;
};

class DA_API DACrateClass abstract {
public:
	virtual void Init();
	virtual void Settings_Loaded();
	virtual void Calculate_Odds(cPlayer *Player);
	virtual bool Can_Activate(cPlayer *Player) { return true; }
	virtual void Activate(cPlayer *Player) = 0;
	virtual ~DACrateClass() { }
	bool Check_Type(cPlayer *Player) const;
	bool Check_Type(DACrateType::Type Type) const;
	void Adjust_Odds(float Odds);
	inline bool Get_Cap_Odds() const {
		return CapOdds;
	}
	inline float Get_Base_Odds() const {
		return BaseOdds;
	}
	inline float Get_Modified_Odds() const {
		return ModifiedOdds;
	}
	inline float Get_Final_Odds() const {
		return FinalOdds;
	}
	inline void Set_Factory(const DACrateFactoryClass *Factory) {
		this->Factory = Factory;
	}
	inline const DACrateFactoryClass *Get_Factory() const {
		return Factory;
	}
	inline const char *Get_Name() const {
		return Factory->Get_Name();
	}
	inline const char *Get_Section() const {
		return Factory->Get_Section();
	}
	inline DACrateType::Type Get_Type() const {
		return Type;
	}
	
protected:
	bool CapOdds;
	float BaseOdds;
	float ModifiedOdds;
	float FinalOdds;
	DACrateType::Type Type;
	DynamicVectorClass <DACrateModifierClass*> Modifiers;
	const DACrateFactoryClass *Factory;
};

template <typename T> class DACrateRegistrant : public DACrateFactoryClass {
public:
	DACrateRegistrant(const char *Name,DACrateType::Type Type) : DACrateFactoryClass(Name,Type) { }
	virtual DACrateClass *Create_Instance() {
		if (!Instance) {
			Instance = new T;
			Instance->Set_Factory(this);
			Instance->Init();
		}
		return Instance;
	}
};

#define Register_Crate(ClassName,Name,Type) DACrateRegistrant<ClassName> ClassName##Registrant(Name,Type);

#endif