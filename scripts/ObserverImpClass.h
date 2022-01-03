/*  Renegade Scripts.dll
    Copyright 2013 Tiberian Technologies
    This file is part of the Renegade scripts.dll
    The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 2, or (at your option) any later
    version. See the file COPYING for more details.
    In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
    Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once

/*!
* \brief GameObject Observer Base Class
* \author Daniel Paul
*
* A base class for a class whose job is to observe a class without actually being a fully fledged
* script (thus not appearing in LE). This is useful when a script wants to passively monitor the
* state of an external object. The only real benefit to using this instead of GameObjObserverClass
* is the dummy implementations of all the abstract methods and the Get_Name() implementation so
* the observer class can simply implement the events it is interested in.
*/
class ObserverImpClass : public GameObjObserverClass
{
public:
  ObserverImpClass(StringClass name) { m_name = name; }
  virtual const char* Get_Name() { return m_name.Peek_Buffer(); }

protected:
  // Provide dummy implementations of all GameObjObserverClass abstracts such that derived classes
  // are not forced to provide their own stubs (that's basically the whole point of this class)
  virtual void Attach(GameObject* obj) {};
  virtual void Detach(GameObject* obj) {};
  virtual void Animation_Complete(GameObject *obj, const char *animation_name) {};
  virtual void Created(GameObject* obj) {};
  virtual void Destroyed(GameObject* obj) {};
  virtual void Killed(GameObject* obj,GameObject* pKiller) {};
  virtual void Damaged(GameObject* obj,GameObject* pDamager,float amount) {};
  virtual void Custom(GameObject* obj,int type,int param,GameObject* pSender) {};
  virtual void Sound_Heard(GameObject* obj,const CombatSound & sound) {};
  virtual void Enemy_Seen(GameObject* obj,GameObject* pEnemy) {};
  virtual void Action_Complete(GameObject* obj,int action_id,ActionCompleteReason complete_reason) {};
  virtual void Timer_Expired(GameObject* obj,int number) {};
  virtual void Poked(GameObject* obj,GameObject* pPoker) {};
  virtual void Entered(GameObject* obj, GameObject* pEnterer) {};
  virtual void Exited(GameObject* obj, GameObject* pExiter) {};

private:
  StringClass m_name;
};
