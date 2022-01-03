#ifndef TT_INCLUDE__OBJECTLIBRARYMANAGER_H
#define TT_INCLUDE__OBJECTLIBRARYMANAGER_H
#include "Persist.h"
class PhysicalGameObj;
class ObjectLibraryManager
{

public:

   static PhysicalGameObj* Create_Object (const char* cPresetName);
   static PhysicalGameObj* Create_Object (uint32 uDefinitionID);

};


#endif
