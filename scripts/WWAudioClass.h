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
#ifndef __WWAUDIOCLASS_H__
#define __WWAUDIOCLASS_H__
#include "engine_common.h"
#include "engine_string.h"
#include "engine_vector.h"
#include "scripts.h"
#include "AudioCallbackListClass.h"
#include "Matrix3D.h"
#include <mmsystem.h>
#include "SoundBufferClass.h"
class AudibleSoundClass;
class Sound3DClass;
class Sound2DTriggerClass;
class StreamSoundClass;
class FileClass;
class SoundSceneClass;
class FileFactoryClass;
class SoundSceneObjClass;
class LogicalListenerClass;
class LogicalSoundClass;
class Matrix3D;
class INIClass;
typedef unsigned int HPROVIDER;
typedef void *HDIGDRIVER;
typedef void *HSAMPLE;
typedef void *H3DSAMPLE;
typedef void *H3DPOBJECT;
typedef void *HSTREAM;
typedef signed int HTIMER;
typedef void (_stdcall  *LPFNSOSCALLBACK)		(SoundSceneObjClass *sound_obj, uint32 user_param);
typedef void (_stdcall  *LPFNEOSCALLBACK)		(SoundSceneObjClass *sound_obj, uint32 user_param);
typedef void (_stdcall  *LPFNHEARDCALLBACK)	(LogicalListenerClass *listener, LogicalSoundClass *sound_obj, uint32 user_param);
typedef void (_stdcall  *LPFNTEXTCALLBACK)	(AudibleSoundClass *sound_obj, const StringClass &text, uint32 user_param);
#define AILCALLBACK WINAPI
typedef enum
{
	CLASSID_UNKNOWN			= 0,
	CLASSID_2D,
	CLASSID_3D,
	CLASSID_LISTENER,
	CLASSID_PSEUDO3D,
	CLASSID_2DTRIGGER,
	CLASSID_LOGICAL,
	CLASSID_FILTERED,
	CLASSID_COUNT
} SOUND_CLASSID;
const int DEF_2D_SAMPLE_COUNT		= 16;
const int DEF_3D_SAMPLE_COUNT		= 16;
const float DEF_MUSIC_VOL			= 1.0F;
const float DEF_SFX_VOL				= 1.0F;
const float DEF_DIALOG_VOL			= 1.0F;
const float DEF_CINEMATIC_VOL		= 1.0F;
const float DEF_FADE_TIME			= 0.5F;
const int DEF_CACHE_SIZE			= 1024;
const int DEF_MAX_2D_BUFFER_SIZE	= 20000;
const int DEF_MAX_3D_BUFFER_SIZE	= 100000;
enum
{
	MAX_CACHE_HASH		= 256,
	CACHE_HASH_MASK	= 0x000000FF
};
class WWAudioClass
{
public:
	typedef enum
	{
		DRIVER2D_ERROR			= 0,
		DRIVER2D_DSOUND,
		DRIVER2D_WAVEOUT,
		DRIVER2D_COUNT
	} DRIVER_TYPE_2D;
	typedef enum
	{
		DRIVER3D_ERROR			= 0,
		DRIVER3D_D3DSOUND,
		DRIVER3D_EAX,
		DRIVER3D_A3D,
		DRIVER3D_RSX,
		DRIVER3D_PSEUDO,
		DRIVER3D_DOLBY,
		DRIVER3D_COUNT
	} DRIVER_TYPE_3D;
	typedef enum
	{
		PAGE_PRIMARY		= 0,
		PAGE_SECONDARY,
		PAGE_TERTIARY,
		PAGE_COUNT
	} SOUND_PAGE;
	typedef struct _DRIVER_INFO_STRUCT
	{
		HPROVIDER	driver;
		char *		name;
	} DRIVER_INFO_STRUCT;
	friend class AudibleSoundClass;
	friend class Sound3DClass;
	friend class Listener3DClass;
	WWAudioClass (bool lite = false);
	virtual ~WWAudioClass (void);
	static WWAudioClass *	Get_Instance (void)		{ return _theInstance; }
	void					Initialize (bool stereo = true, int bits = 16, int hertz = 44100);
	void					Initialize (const char *registry_subkey_name);
	void					Shutdown (void);
	HDIGDRIVER				Get_2D_Driver (void) const			{ return m_Driver2D; }
	HPROVIDER				Get_3D_Driver (void) const			{ return m_Driver3D; }
	const StringClass &	Get_3D_Driver_Name (void) const	{ return m_Driver3DName; }
	HPROVIDER				Get_Reverb_Filter (void) const	{ return m_ReverbFilter; }
	DRIVER_TYPE_2D		Open_2D_Device (LPWAVEFORMAT format = NULL);
	DRIVER_TYPE_2D		Open_2D_Device (bool stereo, int bits, int hertz);
	bool					Close_2D_Device (void);
	int					Get_Playback_Rate (void) const		{ return m_PlaybackRate; }
	int					Get_Playback_Bits (void) const		{ return m_PlaybackBits; }
	bool					Get_Playback_Stereo (void) const		{ return m_PlaybackStereo; }
	int					Get_3D_Device_Count (void) const								{ return m_Driver3DList.Count (); }
	bool					Get_3D_Device (int index, DRIVER_INFO_STRUCT **info)	{ (*info) = m_Driver3DList[index]; return true; }
	bool					Is_3D_Device_Available (DRIVER_TYPE_3D type)				{ return Find_3D_Device (type) >= 0; }
	int					Find_3D_Device (DRIVER_TYPE_3D type);
	bool					Select_3D_Device (int index);
	bool					Select_3D_Device (const char *device_name, HPROVIDER provider);
	bool					Select_3D_Device (DRIVER_TYPE_3D type);
	bool					Select_3D_Device (const char *device_name);
	bool					Close_3D_Device (void);
	void					Set_Speaker_Type (int speaker_type);
	int					Get_Speaker_Type (void) const;
	bool					Load_From_Registry (const char *subkey_name);
	bool					Load_From_Registry (const char *subkey_name, StringClass &device_name, bool &is_stereo, int &bits, int &hertz, bool &sound_enabled, bool &music_enabled, bool &dialog_enabled, bool &cinematic_sound_enabled, float &sound_volume, float &music_volume, float &dialog_volume, float &cinematic_volume, int &speaker_types);
	bool					Save_To_Registry (const char *subkey_name);
	bool					Save_To_Registry (const char *subkey_name, const StringClass &device_name, bool is_stereo, int bits, int hertz, bool sound_enabled, bool music_enabled, bool dialog_enabled, bool cinematic_sound_enabled, float sound_volume, float music_volume, float dialog_volume, float cinematic_volume, int speaker_type);
	void					Load_Default_Volume (int &defaultmusicvolume, int &defaultsoundvolume, int &defaultdialogvolume, int &defaultcinematicvolume);
	void					Set_File_Factory (FileFactoryClass *ffactory)			{ m_FileFactory = ffactory; }
	bool					Set_Max_2D_Sample_Count (int count = DEF_2D_SAMPLE_COUNT) { m_Max2DSamples = count; Allocate_2D_Handles(); return true; } // Reimplementation. No idea whether this matches the original function!
	int					Get_Max_2D_Sample_Count (void) const;
	int					Get_Avail_2D_Sample_Count (void) const;
	bool					Set_Max_3D_Sample_Count (int count = DEF_3D_SAMPLE_COUNT) { m_Max3DSamples = count; Allocate_3D_Handles(); return true; } // Reimplementation. No idea whether this matches the original function!
	int					Get_Max_3D_Sample_Count (void) const;
	int					Get_Avail_3D_Sample_Count (void) const;
	float					Get_Effects_Level (void)	{ return m_EffectsLevel; }
	int					Get_Reverb_Room_Type (void)		{ return m_ReverbRoomType; }
	void					Set_Reverb_Room_Type (int type);
	void					Set_Sound_Effects_Volume (float volume = DEF_SFX_VOL);
	float					Get_Sound_Effects_Volume (void) const						{ return m_SoundVolume; }
	void					Set_Music_Volume (float volume = DEF_MUSIC_VOL);
	float					Get_Music_Volume (void) const									{ return m_MusicVolume; }
	void					Set_Dialog_Volume (float volume = DEF_DIALOG_VOL);
	float					Get_Dialog_Volume (void) const								{ return m_DialogVolume; }
	void					Set_Cinematic_Volume (float volume = DEF_CINEMATIC_VOL);
	float					Get_Cinematic_Volume (void) const							{ return m_CinematicVolume; }
	void					Allow_Sound_Effects (bool onoff = true);
	bool					Are_Sound_Effects_On (void) const							{ return m_AreSoundEffectsEnabled; }
	void					Allow_Music (bool onoff = true);
	bool					Is_Music_On (void) const										{ return m_IsMusicEnabled; }
	void					Allow_Dialog (bool onoff = true);
	bool					Is_Dialog_On (void) const										{ return m_IsDialogEnabled; }
	void					Allow_Cinematic_Sound (bool onoff = true);
	bool					Is_Cinematic_Sound_On (void) const							{ return m_IsCinematicSoundEnabled; }
	void					Enable_New_Sounds (bool onoff)			{ m_AreNewSoundsEnabled = onoff; }
	bool					Are_New_Sounds_Enabled (void)	const		{ return m_AreNewSoundsEnabled; }
	void					Temp_Disable_Audio (bool onoff);
	void					On_Frame_Update (unsigned int milliseconds = 0);
	void					Register_EOS_Callback (LPFNEOSCALLBACK callback, DWORD user_param);
	void					UnRegister_EOS_Callback (LPFNEOSCALLBACK callback);
	void					Register_Text_Callback (LPFNTEXTCALLBACK callback, DWORD user_param);
	void					UnRegister_Text_Callback (LPFNTEXTCALLBACK callback);
	void					Fire_Text_Callback (AudibleSoundClass *sound_obj, const StringClass &text);
	bool					Is_Sound_Cached (const char *string_id);
	AudibleSoundClass *		Create_Sound_Effect (FileClass &file, const char *string_id);
	AudibleSoundClass *		Create_Sound_Effect (const char *filename);
	AudibleSoundClass *		Create_Sound_Effect (const char *string_id, unsigned char *raw_wave_data, unsigned long bytes);
	Sound3DClass *		Create_3D_Sound (FileClass &file, const char *string_id, int classid_hint = CLASSID_3D);
	Sound3DClass *		Create_3D_Sound (const char *filename, int classid_hint = CLASSID_3D);
	Sound3DClass *		Create_3D_Sound (const char *string_id, unsigned char *raw_wave_data, unsigned long bytes, int classid_hint = CLASSID_3D);
	void						Set_Background_Music (const char *filename);
	void						Fade_Background_Music (const char *filename, int fade_out_time, int fade_in_time);
	void						Set_Background_Music_Offset (const char *filename, float startoffset);
	const char *			Get_Background_Music_Name (void)						{ return m_BackgroundMusicName; }
	AudibleSoundClass *	Peek_Background_Music (void)							{ return m_BackgroundMusic; }
	LogicalSoundClass *		Create_Logical_Sound (void);
	LogicalListenerClass *	Create_Logical_Listener (void);
	void							Add_Logical_Type (int id, LPCTSTR display_name);
	void							Reset_Logical_Types (void);
	int							Get_Logical_Type_Count (void) const	{ return m_LogicalTypes.Count (); }
	int							Get_Logical_Type (int index, StringClass &name);
	int						Create_Instant_Sound (int definition_id, const Matrix3D &tm, RefCountClass *user_obj = NULL, uint32 user_data = 0, int classid_hint = CLASSID_3D);
	int						Create_Instant_Sound (const char *def_name, const Matrix3D &tm, RefCountClass *user_obj = NULL, uint32 user_data = 0, int classid_hint = CLASSID_3D);
	AudibleSoundClass *	Create_Continuous_Sound (int definition_id, RefCountClass *user_obj = NULL, uint32 user_data = 0, int classid_hint = CLASSID_3D);
	AudibleSoundClass *	Create_Continuous_Sound (const char *def_name, RefCountClass *user_obj = NULL, uint32 user_data = 0, int classid_hint = CLASSID_3D);
	AudibleSoundClass *	Create_Sound (int definition_id, RefCountClass *user_obj = NULL, uint32 user_data = 0, int classid_hint = CLASSID_3D);
	AudibleSoundClass *	Create_Sound (const char *def_name, RefCountClass *user_obj = NULL, uint32 user_data = 0, int classid_hint = CLASSID_3D);
	SoundSceneObjClass *	Find_Sound_Object (uint32 sound_obj_id);
#if !defined(W3DVIEWER) && !defined(TTLE_EXPORTS)
	SoundSceneObjClass* Find_Server_Sound_Object(uint32 server_sound_obj_id); // Added by TT
	uint32 Get_New_Server_Sound_ID(); // Added by TT
#endif
	SoundSceneClass *	Get_Sound_Scene (void) const		{ return m_SoundScene; }
	bool					Set_Cache_Size (int kbytes = DEF_CACHE_SIZE)	{ m_MaxCacheSize = (kbytes * 1024); }
	int					Get_Cache_Size (void) const						{ return m_MaxCacheSize / 1024; }
	int					Get_Current_Cache_Size (void) const				{ return m_CurrentCacheSize; }
	void					Flush_Cache (void);
	void					Set_Max_2D_Sound_Buffer (int bytes = DEF_MAX_2D_BUFFER_SIZE)	{ m_Max2DBufferSize = bytes; }
	void					Set_Max_3D_Sound_Buffer (int bytes = DEF_MAX_3D_BUFFER_SIZE)	{ m_Max3DBufferSize = bytes; }
	bool					Simple_Play_2D_Sound_Effect (const char *filename, float priority = 1.0F, float volume = DEF_SFX_VOL);
	bool					Simple_Play_2D_Sound_Effect (FileClass &file, float priority = 1.0F, float volume = DEF_SFX_VOL);
	bool						Add_To_Playlist (AudibleSoundClass *sound);
	bool						Remove_From_Playlist (AudibleSoundClass *sound);
	int						Get_Playlist_Count (void) const			{ return m_Playlist[m_CurrPage].Count (); }
	AudibleSoundClass *	Get_Playlist_Entry (int index) const;
	AudibleSoundClass *	Peek_Playlist_Entry (int index) const	{ return m_Playlist[m_CurrPage][index]; }
	void						Flush_Playlist (void);
	void						Flush_Playlist (SOUND_PAGE page);
	bool						Is_Sound_In_Playlist (AudibleSoundClass *sound_obj);
	bool					Acquire_Virtual_Channel (AudibleSoundClass *sound_obj, int channel_index);
	void					Release_Virtual_Channel (AudibleSoundClass *sound_obj, int channel_index);
	void					Set_Active_Sound_Page (SOUND_PAGE page);
	SOUND_PAGE			Get_Active_Sound_Page (void)					{ m_CurrPage; }
	void					Push_Active_Sound_Page (SOUND_PAGE page);
	void					Pop_Active_Sound_Page (void);
	void					Fade_Non_Dialog_In (void);
	void					Fade_Non_Dialog_Out (void);
	void					Set_Non_Dialog_Fade_Time (float fade_time)	{ m_NonDialogFadeTime = fade_time; }
	float					Get_Digital_CPU_Percent (void) const;
	bool						Is_Disabled (void) const;
	int						Get_2D_Sample_Count (void) const	{ return m_2DSampleHandles.Count (); }
	int						Get_3D_Sample_Count (void) const	{ return m_3DSampleHandles.Count (); }
	AudibleSoundClass *	Peek_2D_Sample (int index);
	AudibleSoundClass *	Peek_3D_Sample (int index);
	void					Free_Completed_Sounds (void);
public:
	void						Build_3D_Driver_List (void);
	void						Free_3D_Driver_List (void);
	void						Reprioritize_Playlist (void);
	bool						Validate_3D_Sound_Buffer (SoundBufferClass *buffer);
	FileClass *				Get_File (LPCTSTR filename);
	void						Return_File (FileClass *file);
	void						Allocate_2D_Handles (void);
	void						Release_2D_Handles (void);
	void						Allocate_3D_Handles (void);
	void						Release_3D_Handles (void);
	HSAMPLE					Get_2D_Sample (const AudibleSoundClass &sound_obj);
	H3DSAMPLE				Get_3D_Sample (const Sound3DClass &sound_obj);
	H3DPOBJECT				Get_Listener_Handle (void);
	void						ReAssign_2D_Handles (void);
	void						ReAssign_3D_Handles (void);
	void						Remove_2D_Sound_Handles (void);
	void						Remove_3D_Sound_Handles (void);
	bool						Is_OK_To_Give_Handle (const AudibleSoundClass &sound_obj);
	void						Update_Fade (void);
	void						Internal_Set_Sound_Effects_Volume (float volume);
	void						Internal_Set_Music_Volume (float volume);
	SoundBufferClass *	Get_Sound_Buffer (FileClass &file, const char *string_id, bool is_3d);
	SoundBufferClass *	Get_Sound_Buffer (const char *filename, bool is_3d);
	SoundBufferClass *	Find_Cached_Buffer (const char *string_id);
	SoundBufferClass *	Create_Sound_Buffer (FileClass &file, const char *string_id, bool is_3d);
	SoundBufferClass *	Create_Sound_Buffer (unsigned char *file_image, unsigned long bytes, const char *string_id, bool is_3d);
	bool						Cache_Buffer (SoundBufferClass *buffer, const char *string_id);
	bool						Free_Cache_Space (int bytes);
	static unsigned int AILCALLBACK	File_Open_Callback (char const *filename, unsigned int *file_handle);
	static void AILCALLBACK	File_Close_Callback (unsigned int file_handle);
	static signed int AILCALLBACK	File_Seek_Callback (unsigned int file_handle, signed int offset, unsigned int type);
	static unsigned int AILCALLBACK	File_Read_Callback (unsigned int file_handle, void *buffer, unsigned int bytes);
private:
#ifndef W3DVIEWER
	static REF_DECL(WWAudioClass *, _theInstance);
#else
	static WWAudioClass *_theInstance;
#endif
	static HANDLE									_TimerSyncEvent;
	typedef struct _CACHE_ENTRY_STRUCT : public NoEqualsClass<_CACHE_ENTRY_STRUCT>
	{
		const char* string_id;
		SoundBufferClass* buffer;
	} CACHE_ENTRY_STRUCT;
	typedef struct _LOGICAL_TYPE_STRUCT
	{
		StringClass				display_name;
		int						id;
		_LOGICAL_TYPE_STRUCT (void)
			: id (0) {}
		_LOGICAL_TYPE_STRUCT (int _id, LPCTSTR name)
			:	display_name (name), id (_id) {}
		bool operator== (const _LOGICAL_TYPE_STRUCT &src) { return false; }
		bool operator!= (const _LOGICAL_TYPE_STRUCT &src) { return true; }
	} LOGICAL_TYPE_STRUCT;
	typedef enum
	{
		FADE_NONE	= 0,
		FADE_IN,
		FADE_OUT,
		FADED_OUT,
	}	FADE_TYPE;
	
	int                                      m_PlaybackRate;                   // 0004
	int                                      m_PlaybackBits;                   // 0008
	bool                                     m_PlaybackStereo;                 // 000C
	float                                    m_MusicVolume;                    // 0010
	float                                    m_SoundVolume;                    // 0014
	float                                    m_RealMusicVolume;                // 0018
	float                                    m_RealSoundVolume;                // 001C
	float                                    m_DialogVolume;                   // 0020
	float                                    m_CinematicVolume;                // 0024
	int                                      m_Max2DSamples;                   // 0028
	int                                      m_Max3DSamples;                   // 002C
	int                                      m_Max2DBufferSize;                // 0030
	int                                      m_Max3DBufferSize;                // 0034
	HTIMER                                   m_UpdateTimer;                    // 0038
	bool                                     m_IsMusicEnabled;                 // 003C
	bool                                     m_IsDialogEnabled;                // 003D
	bool                                     m_IsCinematicSoundEnabled;        // 003E
	bool                                     m_AreSoundEffectsEnabled;         // 003F
	bool                                     m_AreNewSoundsEnabled;            // 0040
	FileFactoryClass *                       m_FileFactory;                    // 0044
	AudibleSoundClass *                      m_BackgroundMusic;                // 0048
	StringClass                              m_BackgroundMusicName;            // 004C
	bool                                     m_CachedIsMusicEnabled;           // 0050
	bool                                     m_CachedIsDialogEnabled;          // 0051
	bool                                     m_CachedIsCinematicSoundEnabled;  // 0052
	bool                                     m_CachedAreSoundEffectsEnabled;   // 0053
	AudioCallbackListClass<LPFNEOSCALLBACK>  m_EOSCallbackList;                // 0054
	AudioCallbackListClass<LPFNTEXTCALLBACK> m_TextCallbackList;               // 0064
	SoundSceneClass *                        m_SoundScene;                     // 0074
	SOUND_PAGE                               m_CurrPage;                       // 0078
	DynamicVectorClass<SOUND_PAGE>           m_PageStack;                      // 007C
	HDIGDRIVER                               m_Driver2D;                       // 0094
	HPROVIDER                                m_Driver3D;                       // 0098
	HPROVIDER                                m_Driver3DPseudo;                 // 009C
	HPROVIDER                                m_ReverbFilter;                   // 00A0
	DynamicVectorClass<DRIVER_INFO_STRUCT *> m_Driver3DList;                   // 00A4
	StringClass                              m_Driver3DName;                   // 00BC
	int                                      m_SpeakerType;                    // 00C0
	DynamicVectorClass<HSAMPLE>              m_2DSampleHandles;                // 00C4
	DynamicVectorClass<H3DSAMPLE>            m_3DSampleHandles;                // 00DC
	DynamicVectorClass<AudibleSoundClass *>  m_Playlist[PAGE_COUNT];           // 00F4
	DynamicVectorClass<AudibleSoundClass *>  m_CompletedSounds;                // 013C
	DynamicVectorClass<AudibleSoundClass *>  m_VirtualChannels;                // 0154
	DynamicVectorClass<CACHE_ENTRY_STRUCT>   m_CachedBuffers[MAX_CACHE_HASH];  // 016C
	int                                      m_MaxCacheSize;                   // 196C
	int                                      m_CurrentCacheSize;               // 1970
	DynamicVectorClass<LOGICAL_TYPE_STRUCT>  m_LogicalTypes;                   // 1974
	float                                    m_EffectsLevel;                   // 198C
	int                                      m_ReverbRoomType;                 // 1990
	float                                    m_NonDialogFadeTime;              // 1994
	FADE_TYPE                                m_FadeType;                       // 1998
	float                                    m_FadeTimer;                      // 199C
	INIClass*                                AudioIni;                         // 19A0
	bool                                     m_ForceDisable;                   // 19A4
}; // 19A5
#endif
