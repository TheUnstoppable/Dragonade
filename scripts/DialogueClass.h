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
#ifndef TT_INCLUDE__DIALOGUECLASS_H
#define TT_INCLUDE__DIALOGUECLASS_H
#include "engine_vector.h"
#include "engine_string.h"
class ChunkSaveClass;
class ChunkLoadClass;
class DialogueOptionClass
{
private:
	float Weight;
	int Conversation_ID;
public:
	DialogueOptionClass();
	DialogueOptionClass(DialogueOptionClass const &that);
	virtual ~DialogueOptionClass();
	const DialogueOptionClass& operator=(DialogueOptionClass const &that);
	void Save(ChunkSaveClass& csave);
	void Load(ChunkLoadClass& cload);
	void Load_Variables(ChunkLoadClass& cload);
	int Get_Conversation_ID() const {return Conversation_ID;}
	float Get_Weight() const {return Weight;}
	void Set_Conversation_ID(int id) {Conversation_ID = id;}
	void Set_Weight(float weight) {Weight = weight;}
};
class DialogueClass
{
private:
	DynamicVectorClass<DialogueOptionClass*> DialogueOptions;
	float SilenceWeight;
public:
	DialogueClass();
	DialogueClass(DialogueClass const &that);
	virtual ~DialogueClass();
	const DialogueClass& operator=(const DialogueClass& string);
	void Save(ChunkSaveClass& csave);
	void Load(ChunkLoadClass& cload);
	void Load_Variables(ChunkLoadClass& cload);
	void Free_Options();
	int Get_Conversation();
	DynamicVectorClass<DialogueOptionClass*> &Get_Option_List() {return DialogueOptions;}
	float Get_Silence_Weight() {return SilenceWeight;}
	void Set_Silence_Weight(float weight) {SilenceWeight = weight;}
}; // size: 32
enum DialogueTypes
{
	DIALOG_ON_TAKE_DAMAGE_FROM_FRIEND,
	DIALOG_ON_TAKE_DAMAGE_FROM_ENEMY,
	DIALOG_ON_DAMAGE_FRIEND,
	DIALOG_ON_DAMAGE_ENEMY,
	DIALOG_ON_KILLED_FRIEND,
	DIALOG_ON_KILLED_ENEMY,
	DIALOG_ON_SAW_FRIEND,
	DIALOG_ON_SAW_ENEMY,
	DIALOG_ON_OBSOLETE_01,
	DIALOG_ON_OBSOLETE_02,
	DIALOG_ON_DIE,
	DIALOG_ON_POKE_IDLE,
	DIALOG_ON_POKE_SEARCH,
	DIALOG_ON_POKE_COMBAT,
	DIALOG_ON_IDLE_TO_COMBAT,
	DIALOG_ON_IDLE_TO_SEARCH,
	DIALOG_ON_SEARCH_TO_COMBAT,
	DIALOG_ON_SEARCH_TO_IDLE,
	DIALOG_ON_COMBAT_TO_SEARCH,
	DIALOG_ON_COMBAT_TO_IDLE,
	DIALOG_MAX
};

#endif

