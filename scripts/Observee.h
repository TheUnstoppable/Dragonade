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
#pragma once

#include "Iterator.h"

template<typename T> inline T clone(const T& o) { return o; }

#define TT_SCOPE_DEFINE(d) for (bool c = true; c;) for (d; c; c = false) // Hack, but it should be compiled away anyway.
#define TT_EACH_OBSERVER(observee) TT_CONST_FOREACH(observer, (observee).getObservers()) (**observer)
#define TT_EACH_OBSERVER_SAFE(observee) TT_SCOPE_DEFINE(auto observeeClone = clone((observee).getObservers())) TT_CONST_FOREACH(observer, observeeClone) (**observer)


template<class Observer> class Observee
{
	SimpleDynVecClass<Observer*> observers;

public:
	
	void addObserver(Observer& observer) { observers.Add(&observer); }
	void removeObserver(Observer& observer) { observers.Delete(&observer); }
	SimpleDynVecClass<Observer*>& getObservers() { return observers; }
	bool hasObservers() const { return !observers.isEmpty(); }
};
