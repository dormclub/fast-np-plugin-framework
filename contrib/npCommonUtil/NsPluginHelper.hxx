#ifndef NPSEEDPLUGIN_NSPLUGINHELPER_HXX
#define NPSEEDPLUGIN_NSPLUGINHELPER_HXX

#include <npapi.h>
#include <npruntime.h>

#include <string>

namespace LNpPluginLib
{
   class NPSafeArray;

   class NsPluginHelper
   {
   private:
      enum PROCESS_PROPERTY_TYPE
      {
         PPT_GET,
         PPT_SET,
      };

   public:
      static bool invokeJsFunction(const NPP npp, const char* functionName, NPVariant* vars, int nVarCount, NPVariant* result = NULL);
      static bool invokeJsFunction(const NPP npp, NPObject *npobj, NPSafeArray& vars, NPVariant* result = NULL);
      static std::string extractFuncName(const char* func);

      static bool getNpObjectProperty(NPObject* obj, const char* propertyName, NPVariant* value);
      static bool setNpObjectProperty(NPObject* obj, const char* propertyName, NPVariant* value);
      static bool invokeNpObjectMethod(NPObject* obj,
         const char* propertyName,
         NPSafeArray& vars,
         NPVariant* result);

   private:
      static bool proceeNpObjectProperty(PROCESS_PROPERTY_TYPE type, NPObject* obj, const char* propertyName, NPVariant* value);

   private:
      NsPluginHelper();
      NsPluginHelper(const NsPluginHelper&);
      NsPluginHelper& operator=(const NsPluginHelper&);
   };
}//namespace LNpPluginLib

#endif //#ifndef NPSEEDPLUGIN_NSPLUGINHELPER_HXX