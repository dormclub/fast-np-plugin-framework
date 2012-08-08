#include "NsPluginHelper.hxx"

#include "NPSafeArray.hxx"

#include <util/StlUtil.hxx>
//#include <tutil/log/Logger.hxx>
#include <nscore.h>
#include <assert.h>

#define DEFAULT_SUBSYSTEM "NS_PLUGIN"

using namespace LNpPluginLib;

//public
bool NsPluginHelper::invokeJsFunction(const NPP npp,
                                      const char* functionName,
                                      NPVariant* vars,
                                      int nVarCount,
                                      NPVariant* result)
{
   bool re = false;

   if (!functionName || NULL == npp)
   {
      return re;
   }

   NPObject* sWindowObj = nsnull;
   NPError npError = NPN_GetValue(npp, NPNVWindowNPObject, &sWindowObj);
   if (npError == NPERR_NO_ERROR)
   {
      NPIdentifier jsFunctionID = NPN_GetStringIdentifier(functionName);
      if (jsFunctionID)
      {
         NPVariant* rval = result;
         NPVariant newValue;
         if (NULL == rval)
         {
            rval = &newValue;
         }

         if (nVarCount == 0)
         {
            re = NPN_Invoke(npp, sWindowObj, jsFunctionID, NULL, 0, rval);
         }
         else if (vars)
         {
            re = NPN_Invoke(npp, sWindowObj, jsFunctionID, vars, nVarCount, rval);
         }
         else
         {
            assert("invokeJsFunction failed!");
         }

         if (re)
         {
            NPN_ReleaseVariantValue(rval);
         }
      }
   }

   return re;
}

//public
bool NsPluginHelper::invokeJsFunction(const NPP npp,
                                      NPObject *npobj,
                                      NPSafeArray& vars,
                                      NPVariant* result)
{
   //DebugLog(<< __FUNCTION__);
   bool re = false;

   if (npobj)
   {
      NPVariant* rval = result;
      NPVariant newValue;
      if (NULL == rval)
      {
         rval = &newValue;
      }

      if (vars.isEmpty())
      {
         NPVariant nullVar[1];
         NULL_TO_NPVARIANT(nullVar[0]);
         re = NPN_InvokeDefault(npp, npobj, nullVar, 1, rval);
      }
      else
      {
         //DebugLog(<< __FUNCTION__<<" | call NPN_InvokeDefault");
         re = NPN_InvokeDefault(npp, npobj, vars, vars.size(), rval);
         //DebugLog(<< __FUNCTION__<<" | call NPN_InvokeDefault end");
      }

      if (re)
      {
         NPN_ReleaseVariantValue(rval);
      }
   }

   return re;
}

//public
std::string NsPluginHelper::extractFuncName(const char* func) // funct : function OnInitialized()
{
   std::string originalFunc(LNpPluginLib::StringUtil::sEmpty);

   if (LNpPluginLib::StringUtil::NotNullOrEmpty(func))
   {
      originalFunc = func;

      std::string beginFlag("function");
      std::string::size_type nFuncPos = originalFunc.find(beginFlag);
      if (nFuncPos != std::string::npos)
      {
         originalFunc = originalFunc.substr(nFuncPos + beginFlag.length(), originalFunc.length());

         nFuncPos = originalFunc.find("(");
         if (nFuncPos != std::string::npos)
         {
            originalFunc = originalFunc.substr(0, nFuncPos);

            originalFunc.erase(0, originalFunc.find_first_not_of(" \t\n\r"));
            originalFunc.erase(originalFunc.find_last_not_of(" \t\n\r") + 1);
         }
         else
         {
            originalFunc = LNpPluginLib::StringUtil::sEmpty;
         }
      }
      else
      {
         originalFunc = LNpPluginLib::StringUtil::sEmpty;
      }
   }

   return originalFunc;
}

//public
bool NsPluginHelper::getNpObjectProperty(NPObject* obj, const char* propertyName, NPVariant* value)
{
   return proceeNpObjectProperty(PPT_GET, obj, propertyName, value);
}

//public
bool NsPluginHelper::setNpObjectProperty(NPObject* obj, const char* propertyName, NPVariant* value)
{
   return proceeNpObjectProperty(PPT_SET, obj, propertyName, value);
}

//public
bool NsPluginHelper::invokeNpObjectMethod(NPObject* obj, const char* propertyName,
                                          NPSafeArray& vars,
                                          NPVariant* result)
{
   bool rt = false;

   if (obj && LNpPluginLib::StringUtil::NotNullOrEmpty(propertyName))
   {
      NPClass* objClass = obj->_class;
      if (nsnull != objClass)
      {
         NPIdentifier nameIdentifier = NPN_GetStringIdentifier(propertyName);
         if (nsnull != nameIdentifier)
         {
            bool hasMethod = objClass->hasMethod(obj, nameIdentifier);
            if (hasMethod)
            {
               rt = objClass->invoke(obj, nameIdentifier, vars, vars.size(), result);
            }
         }
      }
   }

   return rt;
}
//------------------------------------------------------------------------------------------------------------------
//private
bool NsPluginHelper::proceeNpObjectProperty(PROCESS_PROPERTY_TYPE type,
                                            NPObject* obj,
                                            const char* propertyName,
                                            NPVariant* value)
{
   bool result = false;

   if (obj && value && LNpPluginLib::StringUtil::NotNullOrEmpty(propertyName))
   {
      NPClass* objClass = obj->_class;
      if (nsnull != objClass)
      {
         NPIdentifier nameIdentifier = NPN_GetStringIdentifier(propertyName);
         if (nsnull != nameIdentifier)
         {
            bool hasProperty = objClass->hasProperty(obj, nameIdentifier);
            if (hasProperty)
            {
               switch(type)
               {
               case PPT_SET:
                  result = objClass->setProperty(obj, nameIdentifier, value);
                  break;
               case PPT_GET:
                  result = objClass->getProperty(obj, nameIdentifier, value);
                  break;
               }
            }
         }
      }
   }

   return result;
}
//------------------------------------------------------------------------------------------------------------------
