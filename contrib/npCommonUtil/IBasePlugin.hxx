#ifndef NP_RUNTIME_BASE_PLUGIN_H_
#define NP_RUNTIME_BASE_PLUGIN_H_

#include <npapi.h>
#include <npruntime.h>
//---------------------------------------------------------
namespace LNpPluginLib
{
   class IBasePlugin
   {
   public:
      virtual ~IBasePlugin() {}

   public:
      virtual NPBool init(NPWindow* pNPWindow) = 0;
      virtual void shut() = 0;
      virtual NPBool isInitialized() const = 0;
      virtual int16 handleEvent(void* event) = 0;
      virtual NPObject* GetScriptableObject() = 0;
   };
}//namespace LNpPluginLib

#endif //#ifndef NP_RUNTIME_BASE_PLUGIN_H_