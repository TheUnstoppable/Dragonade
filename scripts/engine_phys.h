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
#ifndef SCRIPTS_INCLUDE__ENGINE_PHYS_H
#define SCRIPTS_INCLUDE__ENGINE_PHYS_H
class Vector3;
class OBBoxClass;
const char SCRIPTS_API *Get_Model(GameObject *obj); //get the name of the 3d model used by an object,opposite of Set_Model
float SCRIPTS_API Get_Animation_Frame(GameObject *obj); //gets the current animation frame for obj,may not work for all objects
float SCRIPTS_API Get_Animation_Target_Frame(GameObject *obj); //gets the target animation frame for obj,may not work for all objects
void SCRIPTS_API Copy_Transform(GameObject *in,GameObject *out); //copies the complete transform (including rotation) from one object to another
float SCRIPTS_API Get_Mass(GameObject *obj); //returns the mass of an object
const char SCRIPTS_API *Get_Htree_Name(GameObject *obj); //returns the Hierarchy Tree name for an object
char SCRIPTS_API Get_Sex(GameObject *obj); //gets the character (e.g. "A" for male or "B" for female) for use with animations
void SCRIPTS_API Create_Effect_All_Stealthed_Objects_Area(const Vector3 &Position,float Distance,const char *object,const Vector3 &offset,int team); //creates an indicator object near every stealthed object in a given area
void SCRIPTS_API Create_Effect_All_Of_Preset(const char *object,const char *preset,float ZAdjust,bool ZSet); //Create an object above all objects of a given preset, also set the facing to match the object its being created over.
void SCRIPTS_API Wake_Up_Objects_In_Box(GameObject *obj,const char *box);
void SCRIPTS_API Wake_Up_Objects_In_OBBox(OBBoxClass &BoundingBox);

/*!
* \brief Get current object animation
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* Get the name of the animation currently playing on the specified object, if any
*
* \param[in] pObj
*   Pointer to the GameObject for which to get the current animation name
*
* \return
*   The name of the current animation playing on the specified object or NULL if the object doesn't
*   support animations or is not currently playing one
*/
SCRIPTS_API const char* Get_Animation_Name ( GameObject* pObj );

#endif
