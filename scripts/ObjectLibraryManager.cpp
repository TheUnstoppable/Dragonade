#include "general.h"

#include "ObjectLibraryManager.h"
#include "DefinitionMgrClass.h"
#include "Definition.h"
PhysicalGameObj* ObjectLibraryManager::Create_Object
   (const char* cPresetName)
{
   // const DefinitionClass* oDefinition = DefinitionMgrClass::Find_Typed_Definition (cPresetName, 12288, true);
   DefinitionClass* oDefinition = DefinitionMgrClass::Find_Named_Definition (cPresetName, true);

   StringClass oError;
   if (!oDefinition || !oDefinition->Is_Valid_Config (oError))
      return 0;

   return (PhysicalGameObj *)oDefinition->Create();
}



PhysicalGameObj* ObjectLibraryManager::Create_Object
   (uint32 uDefinitionID)
{
   DefinitionClass* oDefinition = DefinitionMgrClass::Find_Definition (uDefinitionID, true);

   StringClass oError;
   if (!oDefinition || !oDefinition->Is_Valid_Config (oError))
      return 0;

   return (PhysicalGameObj *)oDefinition->Create();
}
