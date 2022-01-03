/*  Renegade Scripts.dll
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

#include "NetworkObjectClass.h"
#include "DataSafe.h"
#include "Persist.h"

class PhysicalGameObj;
class VehicleGameObj;
class SmartGameObj;
class ScriptableGameObj;
class DefinitionClass;
class BaseGameObjDef;

class BaseGameObj: public PersistClass, public NetworkObjectClass
{
public:

    BaseGameObj();
    virtual ~BaseGameObj();

    virtual void                Init() = 0;
    void                        Init(const BaseGameObjDef & definition);

    virtual bool                Save(ChunkSaveClass& csave);
    virtual bool                Load(ChunkLoadClass& cload);
    virtual void                Think() { IsPostThinkAllowed = true; }
    virtual void                Post_Think() {};
    virtual bool                Is_Hibernating() { return false; }

    virtual PhysicalGameObj*    As_PhysicalGameObj() { return nullptr; }
    virtual VehicleGameObj*     As_VehicleGameObj() { return nullptr; }
    virtual SmartGameObj*       As_SmartGameObj() { return nullptr; }
    virtual ScriptableGameObj*  As_ScriptableGameObj() { return nullptr; }

    virtual uint32              Get_Network_Class_ID() const { return 0x3E8; }
    virtual void                Delete() { delete this; }

    const BaseGameObjDef&       Get_Definition() const { return *Definition; };
    void                        Set_ID(int id) { Set_Network_ID(id); }
    int                         Get_ID() const { return Get_Network_ID(); }

    bool                        Is_Post_Think_Allowed() { return IsPostThinkAllowed; }
    void                        Enable_Cinematic_Freeze(bool enable) { EnableCinematicFreeze = enable; }
    bool                        Is_Cinematic_Freeze_Enabled() { return EnableCinematicFreeze; }

//private: //DA

    const BaseGameObjDef*       Definition;             // 06BC
    bool                        IsPostThinkAllowed;     // 06C0
    bool                        EnableCinematicFreeze;  // 06C1

};// 06C2
