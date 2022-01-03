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

#include "scripts.h"

/*!
* \brief Controller for looped animations using a subset of frames
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This class is designed to be a generic controller that can be dropped into a script to control
* looping animations that do not use the entire set of frames within an animation. By default the
* animation subsystem in the Renegade engine does not support this functionality and previously
* the code in this class was duplicated across several scripts that required it.
*
* It works by attaching itself as a listener on the object being animated, thus causing the
* Animation_Complete function to be called each time the object completes an animation from a
* start frame to an end frame. It can then restart that animation from the start frame again, thus
* creating a looping animation.
*/
class LoopedAnimationController : public GameObjObserverClass
{
public:
  /*! Class constructor - adds itself as a listener to the specified object
  *
  * \param[in] pObj
  *   The GameObject upon which we want to control looping animations
  */
  LoopedAnimationController ( GameObject* pObj );

  /*! Class destructor - removes itself as a listener if the object still exists */
  ~LoopedAnimationController();


  /*! Start playing a looped animation on the object we are listening to
  *
  * \param[in] animation
  *   Name of the animation to play
  * \param[in] startFrame
  *   First frame number within the animation to be played
  * \param[in] endFrame
  *   Final frame number within the animation to be played, before looping back to startFrame
  * \param[in] nRepetitions
  *   Optional number of repetitions to play before stopping the animation, or 0 to loop indefinitely
  */
  void PlayAnimation ( const char* animation, int startFrame, int endFrame, int nRepetitions = 0 );

  /*! Stop the currently playing looped animation on the object, if any */
  void StopAnimation();


protected:
  // These are observer events we actually use
  virtual void Attach ( GameObject* pObj );
  virtual void Detach ( GameObject* pObj );
  virtual void Animation_Complete ( GameObject *pObj, const char *animation_name );


  // These are observer events we don't use but have to provide an implementation for to compile
  virtual void Created(GameObject* pObj) {};
  virtual void Destroyed(GameObject* pObj) {};
  virtual void Killed(GameObject* pObj,GameObject* pKiller) {};
  virtual void Damaged(GameObject* pObj,GameObject* pDamager,float amount) {};
  virtual void Custom(GameObject* pObj,int type,int param,GameObject* pSender) {};
  virtual void Sound_Heard(GameObject* pObj,const CombatSound & sound) {};
  virtual void Enemy_Seen(GameObject* pObj,GameObject* pEnemy) {};
  virtual void Action_Complete(GameObject* pObj,int action_id,ActionCompleteReason complete_reason) {};
  virtual void Timer_Expired(GameObject* pObj,int number) {};
  virtual void Poked(GameObject* pObj,GameObject* pPoker) {};
  virtual void Entered(GameObject* pObj,GameObject* pEnterer) {};
  virtual void Exited(GameObject* pObj,GameObject* pExiter) {};


  // We also need to provide an implementation for Get_Name to compile
  virtual const char* Get_Name() { return "LoopedAnimationController"; }



private:
  /*! ID of the GameObject to which we are listening */
  int m_objID;

  /*! Name of the looped animation we are currently playing, or NULL if we are not currently playing
  * a looped animation */
  char* m_animationName;

  /*! Start frame of the looped animation */
  int m_startFrame;

  /*! End frame of the looped animation */
  int m_endFrame;

  /*! Number of loop repetitions remaining before the looping animation is stopped, or 0 if there
  was no maximum number of repetitions set when starting the animation */
  int m_nRepetitions;
};