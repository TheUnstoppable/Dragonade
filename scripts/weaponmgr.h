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
#ifndef TT_INCLUDE_WEAPONMGR_H
#define TT_INCLUDE_WEAPONMGR_H
#include "Definition.h"
#include "Rect.h"
#include "DataSafe.h"
#include "Vector3.h"
class AmmoDefinitionClass : public DefinitionClass {
public:
	enum {
		AMMO_TYPE_NORMAL,
		AMMO_TYPE_C4_REMOTE,
		AMMO_TYPE_C4_TIMED,
		AMMO_TYPE_C4_PROXIMITY,
	};
	AmmoDefinitionClass( void );
	virtual uint32								Get_Class_ID( void ) const;
	virtual PersistClass *					Create( void ) const					{return NULL; }
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
	virtual const PersistFactoryClass &	Get_Factory( void ) const;
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
#endif
	DECLARE_EDITABLE( AmmoDefinitionClass, DefinitionClass );
	bool operator == ( const AmmoDefinitionClass & vector) const { return false; }
	bool operator != ( const AmmoDefinitionClass & vector) const	{ return true; }
	StringClass		ModelFilename;
	StringClass		ModelName;
	safe_int			Warhead;
	safe_float		Damage;
	safe_float		Range;
	safe_float		Velocity;
	float				Gravity;
	float				Elasticity;
	float				RateOfFire;
	float				SprayAngle;
	safe_int			SprayCount;
	StringClass		TrailEmitter;
	float				AquireTime;
	float				BurstDelayTime;
	safe_int			BurstMax;
	int				SoftPierceLimit;
	float				TurnRate;
	bool				TimeActivated;
	bool				TerrainActivated;
	bool				IsTracking;
	safe_float		EffectiveRange;
	int				ExplosionDefID;
	float				RandomTrackingScale;
	bool				DisplayLaser;
	safe_float		ChargeTime;
	int				ContinuousSoundDefID;
	int				FireSoundDefID;
	StringClass		ContinuousEmitterName;
	int				MaxBounces;
	int				SprayBulletCost;
	safe_int			AmmoType;
	float				C4TriggerTime1;
	float				C4TriggerTime2;
	float				C4TriggerTime3;
	float				C4TriggerRange1;
	float				C4TriggerRange2;
	float				C4TriggerRange3;
	int				C4TimingSound1ID;
	int				C4TimingSound2ID;
	int				C4TimingSound3ID;
	float				AliasedSpeed;
	int				HitterType;
	int				BeaconDefID;
	bool				BeamEnabled;
	Vector3			BeamColor;
	float				BeamTime;
	float				BeamWidth;
	bool				BeamEndCaps;
	StringClass		BeamTexture;
	bool				BeamSubdivisionEnabled;
	float				BeamSubdivisionScale;
	bool				BeamSubdivisionFrozen;
	int				IconNameID;
	StringClass		IconTextureName;
	RectClass		IconTextureUV;
	Vector2			IconOffset;
	float			GrenadeSafetyTime;
	bool            C4Animation;
	safe_float		TargetingRange;//new
	int             C4Preset;
	bool			DisableHitscan;
	bool			TrackingIgnoreVehicle;
	bool			TrackingIgnoreSoldier;
	bool			TrackingIgnoreVTOL;
	bool			UseLighting;

#if (PARAM_EDITING_ON) || (DDBEDIT) || (W3DVIEWER)
    Vector3 ProjectileExtent;
#else
    SafeDataClass<Vector3> ProjectileExtent;
#endif

	friend class PresetDump;
}; // 0110

class	WeaponDefinitionClass : public DefinitionClass {
public:
	WeaponDefinitionClass( void );
	virtual uint32								Get_Class_ID( void ) const;
	virtual PersistClass *					Create( void ) const					{ return NULL; }
	virtual bool								Save( ChunkSaveClass &csave );
	virtual bool								Load( ChunkLoadClass &cload );
	virtual const PersistFactoryClass &	Get_Factory( void ) const;
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
	virtual void                        DumpPhys (FileClass &file);
#endif
	DECLARE_EDITABLE( WeaponDefinitionClass, DefinitionClass );
	bool operator == ( const WeaponDefinitionClass & vector) const  { return false; }
	bool operator != ( const WeaponDefinitionClass & vector) const  { return true; }
	int				Style;
	StringClass		Model;
	StringClass		IdleAnim;
	StringClass		FireAnim;
	StringClass		BackModel;
	float				SwitchTime;
	safe_float		ReloadTime;
	float				KeyNumber;
	bool				CanSnipe;
	bool				CanReceiveGenericCnCAmmo;
	float				Rating;
	int				EjectPhysDefID;
	int				MuzzleFlashPhysDefID;
	StringClass		FirstPersonModel;
	Vector3			FirstPersonOffset;
	float				RecoilImpulse;
	StringClass		HUDIconTextureName;
	int				ReloadSoundDefID;
	int				EmptySoundDefID;
	int				PrimaryAmmoDefID;
	int				SecondaryAmmoDefID;
	safe_int			ClipSize;
	float				RecoilTime;
	float				RecoilScale;
	bool				AGiveWeaponsWeapon;
	safe_int			MaxInventoryRounds;
	int				IconNameID;
	StringClass		IconTextureName;
	RectClass		IconTextureUV;
	Vector2			IconOffset;
	StringClass		HumanFiringAnimation;
	StringClass     ScopeTexture;
	float           MaxZoom;
	float           MinZoom;
	int				GlobalFireSoundDefID;
	bool            AreaEffectWeapon;
	bool			AreaEffectWeaponAffectOwner;
	bool			AreaEffectWeaponAffectFriendly;
	bool			AreaEffectWeaponAffectEnemy;
	bool			AreaEffectWeaponAffectNeutral;
	StringClass		FirstPersonAnim;
	bool            DoTiltWhileReloading;
	bool			AllowEmptyShots;
	friend class PresetDump;
}; // 00AC

class ExplosionDefinitionClass : public DefinitionClass {
public:
#ifdef DDBEDIT
	virtual void                        Dump (FileClass &file);
	virtual void                        DumpPhys (FileClass &file);
#endif
	DECLARE_EDITABLE(ExplosionDefinitionClass,DefinitionClass);
	int PhysDefID;
	uint32 SoundDefID;
	float DamageRadius;
	float DamageStrength;
	int Warhead;
	bool DamageIsScaled;
	StringClass DecalFilename;
	float DecalSize;
	bool AnimatedExplosion;
	float CameraShakeIntensity;
	float CameraShakeRadius;
	float CameraShakeDuration;
	float CollisionDamageMultiplier;
	bool ResetMappers;
	ExplosionDefinitionClass();
	virtual ~ExplosionDefinitionClass();
	virtual const PersistFactoryClass &Get_Factory() const;
	virtual bool Save(ChunkSaveClass &csave);
	virtual bool Load(ChunkLoadClass &cload);
	virtual uint32 Get_Class_ID() const;
	virtual PersistClass *Create() const;
	friend class PresetDump;
};

#endif