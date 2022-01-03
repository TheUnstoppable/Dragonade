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
#ifndef __SOUNDSCENEOBJCLASS_H__
#define __SOUNDSCENEOBJCLASS_H__

#include "engine_threading.h"
#include "engine_string.h"
#include "engine_vector.h"
#include "audiocallback.h"
#include "Persist.h"
#include "Matrix3D.h"
#if !defined(W3DVIEWER) && !defined(TTLE_EXPORTS)
#include "HashTemplateClass.h"
#endif
class SoundSceneClass;
class SoundCullObjClass;
class Sound3DClass;
class SoundPseudo3DClass;
class FilteredSoundClass;
class Listener3DClass;
class AudibleSoundClass;
class RenderObjClass;
const uint32	SOUND_OBJ_DEFAULT_ID	= 0;
const uint32	SOUND_OBJ_START_ID	= 1000000000;

class SoundSceneObjClass : public MultiListObjectClass, public PersistClass, public RefCountClass
{
public:
	friend class SoundSceneClass;
	friend class WWAudioClass;
	friend class HandleMgrClass;
	SoundSceneObjClass (void);
	SoundSceneObjClass (const SoundSceneObjClass &src);
	virtual ~SoundSceneObjClass (void);
	const SoundSceneObjClass &operator= (const SoundSceneObjClass &src);
	virtual Sound3DClass *			As_Sound3DClass (void) 			{ return NULL; }
	virtual SoundPseudo3DClass *	As_SoundPseudo3DClass (void) 	{ return NULL; }
	virtual FilteredSoundClass *	As_FilteredSoundClass (void) 	{ return NULL; }
	virtual Listener3DClass *		As_Listener3DClass (void) 		{ return NULL; }
	virtual AudibleSoundClass *	As_AudibleSoundClass(void) 	{ return NULL; }
	virtual uint32			Get_ID (void) const	{ return m_ID; }
	virtual void			Set_ID (uint32 id);
	virtual bool			On_Frame_Update (unsigned int milliseconds = 0);
	virtual void			On_Event (AudioCallbackClass::EVENTS event, uint32 param1 = 0, uint32 param2 = 0);
	virtual void			Register_Callback (AudioCallbackClass::EVENTS events, AudioCallbackClass *callback);
	virtual void			Remove_Callback (void);
	virtual void			Set_Position (const Vector3 &position)	= 0;
	virtual Vector3		Get_Position (void) const = 0;
	virtual void			Set_Listener_Transform (const Matrix3D &tm) {};
	virtual void			Set_Transform (const Matrix3D &transform) = 0;
	virtual Matrix3D		Get_Transform (void) const = 0;
	virtual void			Cull_Sound (bool culled = true)	= 0;
	virtual bool			Is_Sound_Culled (void) const		= 0;
	virtual void			Set_User_Data (RefCountClass *user_obj = NULL, uint32 user = 0)	{ REF_PTR_SET (m_UserObj, user_obj); m_UserData = user; }
	virtual uint32			Get_User_Data (void) const														{ return m_UserData; }
	virtual RefCountClass *Peek_User_Obj (void) const													{ return m_UserObj; }
	virtual void				Attach_To_Object (RenderObjClass *render_obj, int bone_index = -1);
	virtual void				Attach_To_Object (RenderObjClass *render_obj, const char *bone_name);
	virtual RenderObjClass *Peek_Parent_Object (void)			{ return m_AttachedObject; }
	virtual int					Get_Parent_Bone (void)				{ return m_AttachedBone; }
	virtual void				Apply_Auto_Position (void);
	virtual void			Add_To_Scene (bool start_playing = true) = 0;
	virtual void			Remove_From_Scene (void) = 0;
	virtual bool			Is_In_Scene (void) const			{ return m_Scene != NULL; }
	virtual void			Set_DropOff_Radius (float radius = 1) = 0;
	virtual float			Get_DropOff_Radius (void) const = 0;
	bool						Save (ChunkSaveClass &csave);
	bool						Load (ChunkLoadClass &cload);
protected:
	virtual SoundCullObjClass *	Peek_Cullable_Wrapper (void) const					{ return m_PhysWrapper; }
	virtual void						Set_Cullable_Wrapper (SoundCullObjClass *obj)	{ m_PhysWrapper = obj; }
	static void				Register_Sound_Object (SoundSceneObjClass *sound_obj);
	static void				Unregister_Sound_Object (SoundSceneObjClass *sound_obj);		
	static bool				Find_Sound_Object (uint32 id_to_find, int *index);

	SoundSceneClass *				m_Scene; // 0018  0030
	SoundCullObjClass *			m_PhysWrapper; // 001C  0034
	AudioCallbackClass *			m_pCallback; // 0020  0038
	AudioCallbackClass::EVENTS	m_RegisteredEvents; // 0024  003C
	uint32							m_ID; // 0028  0040
	RenderObjClass *				m_AttachedObject; // 002C  0044
	int								m_AttachedBone; // 0030  0048
	uint32							m_UserData; // 0034  004C
	RefCountClass *				m_UserObj; // 0038  0050

	static uint32 m_NextAvailableID;
	static DynamicVectorClass<SoundSceneObjClass *>	m_GlobalSoundList;
	static CriticalSectionClass m_IDListMutex;

	// Added by TT
#if !defined(W3DVIEWER) && !defined(TTLE_EXPORTS)
public:
	static void Associate_Server_ID(SoundSceneObjClass* sound_obj, uint32 server_id);

protected:
	static HashTemplateClass<uint32, SoundSceneObjClass*> m_ServerGlobalSoundList;
	static HashTemplateClass<uint32, uint32> m_ReversedServerGlobalSoundList;
	static CriticalSectionClass m_ServerIDListMutex;

	static bool Find_Server_Sound_Object(uint32 server_id_to_find, SoundSceneObjClass*& sound_obj);
#endif
}; // 003C  0054

#endif
