/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE__AUDIBLESOUNDCLASS_H
#define TT_INCLUDE__AUDIBLESOUNDCLASS_H



#include "Matrix3D.h"
#include "SoundSceneObjClass.h"
#include "WWAudioClass.h"


class AudibleSoundDefinitionClass;
class SoundHandleClass;
class SoundBufferClass;



class AudibleSoundClass : public SoundSceneObjClass
{

public:

   enum Fade
   {
      NoFade  = 0,
      FadeIn  = 1,
      FadeOut = 2,
   };

   enum SoundType
   {
      TypeMusic     = 0,
      TypeEffect    = 1,
      TypeDialog    = 2,
      TypeCinematic = 3,
   };

protected:

   SoundHandleClass*   handle;            //  84  54
   uint32              time;              //  88  58
   uint32              state;             //  92  5C
   SoundType                type;              //  96  60
   Fade                fade;              // 100  64
   uint32              fadeRemainingTime; // 104  68
   uint32              fadeTime;          // 108  6C
   uint32              channel;           // 112  70
   SoundBufferClass*   buffer;            // 116  74
   float               runtimePriority;   // 120  78
   float               priority;          // 124  7C
   float               realVolume;        // 128  80
   float               volume;            // 132  84
   float               pan;               // 136  88
   uint32              loopCount;         // 140  8C
   int32               remainingLoops;    // 144  90
   uint32              duration;          // 148  94
   uint32              playPosition;      // 152  98
   float               startPosition;     // 156  9C
   float               pitch;             // 160  A0
   Matrix3D            listenerTransform; // 164  A4
   Matrix3D            transform;         // 212  D4
   Matrix3D            o260; //104
   bool                culled;            // 308 134
   bool                dirty;             // 309 135
   float               dropOffRadius;     // 312 138
   FilteredSoundClass* filteredSound;     // 316 13C
   AudibleSoundDefinitionClass*definition;        // 320 140
   LogicalSoundClass*  logicalSound;      // 324 144


public:

           AudibleSoundClass();
  virtual ~AudibleSoundClass();

  virtual const PersistFactoryClass &Get_Factory             () const;
  virtual bool Save (ChunkSaveClass& xSave);
  virtual bool Load (ChunkLoadClass& xLoad);
  virtual AudibleSoundClass*  As_AudibleSoundClass    ();
  virtual bool                On_Frame_Update         (uint32 timePast);
  virtual void                Set_Position            (const Vector3& position);
  virtual Vector3             Get_Position            () const;
  virtual void                Set_Listener_Transform  (const Matrix3D& listenerTransform);
  virtual void                Set_Transform           (const Matrix3D& transform);
  virtual Matrix3D            Get_Transform           () const;
  virtual void                Cull_Sound              (bool cull);
  virtual bool                Is_Sound_Culled         () const;
  virtual void                Add_To_Scene            (bool);
  virtual void                Remove_From_Scene       ();
  virtual void                Set_DropOff_Radius      (float dropOffRadius);
  virtual float               Get_DropOff_Radius      () const;
  virtual unsigned long       Get_Class_ID            () const;
  virtual AudibleSoundClass::SoundType           Get_Type                () const;
  virtual void                Set_Type                (SoundType type);
  virtual bool    Play                    (bool addToPlaylist);
  virtual bool                Pause                   ();
  virtual bool                Resume                  ();
  virtual bool                Stop                    (bool removeFromPlaylist);
  virtual void                Seek                    (uint32 playPosition);
  virtual uint32              Get_State               () const;
  virtual void                Fade_Out                (uint32 fadeTime);
  virtual void                Fade_In                 (uint32 fadeTime);
  virtual uint32              Get_Timestamp           () const;
  virtual uint32              Get_Virtual_Channel     () const;
  virtual void                Set_Virtual_Channel     (uint32 channel);
  virtual const char*         Get_Filename            () const;
  virtual bool                Is_Playing              () const;
  virtual float               Get_Pan                 () const;
  virtual void                Set_Pan                 (float pan);
  virtual float               Get_Volume              () const;
  virtual void                Set_Volume              (float volume);
  virtual void                Update_Volume           ();
  virtual uint32              Get_Loop_Count          () const;
  virtual uint32              Get_Loops_Left          () const;
  virtual void                Set_Loop_Count          (uint32 loopCount);
  virtual float               Get_Priority            () const;
  virtual float               Peek_Priority           () const;
  virtual void                Set_Priority            (float priority);
  virtual float               Get_Runtime_Priority    () const;
  virtual void                Set_Runtime_Priority    (float runtimePriority);
  virtual uint32              Get_Playback_Rate       () const;
  virtual void                Set_Playback_Rate       (uint32 rate);
  virtual float               Get_Pitch_Factor        () const;
  virtual void                Set_Pitch_Factor        (float pitch);
  virtual uint32              Get_Duration            () const;
  virtual uint32              Get_Play_Position       () const;
  virtual void                Set_Play_Position       (float playPosition);
  virtual void                Set_Play_Position       (uint32 playPosition);
  virtual void                Set_Start_Offset        (float startPosition);
  virtual float               Get_Start_Offset        () const;
  virtual void                Set_Dirty               (bool dirty);
  virtual bool                Is_Dirty                () const;
  virtual void                Set_Definition          (AudibleSoundDefinitionClass* definition);
  virtual AudibleSoundDefinitionClass *Get_Definition ();
  virtual void                Re_Sync                 (const AudibleSoundClass& sound);
  virtual void                Free_Conversion         ();
  virtual void                Convert_To_Filtered     ();
  virtual FilteredSoundClass* As_Converted_Format     ();
  virtual float               Determine_Real_Volume   () const;
  virtual void                Internal_Set_Volume     (float volume);
  virtual void                Update_Fade             ();
  virtual SoundHandleClass*   Get_Miles_Handle        () const;
  virtual void                Set_Miles_Handle        (HSAMPLE milesHandle);
  virtual void                Free_Miles_Handle       ();
  virtual void                Initialize_Miles_Handle ();
  virtual void                Allocate_Miles_Handle   ();
  virtual SoundBufferClass*   Get_Buffer              () const;
  virtual SoundBufferClass*   Peek_Buffer             () const;
  virtual void                Set_Buffer              (SoundBufferClass* buffer);
  virtual void                Restart_Loop            ();
  virtual void                Update_Play_Position    ();
  virtual void                On_Loop_End             ();

  bool Verify_Playability();

  AudibleSoundClass& operator = (const AudibleSoundClass& sound);

}; // 328 148


#endif
