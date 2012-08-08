#ifndef NPSEEDPLUGIN_NPOBJECTWRAPPER_HXX
#define NPSEEDPLUGIN_NPOBJECTWRAPPER_HXX

#include "IdentifierDict.hxx"
#include "NPVariantHelper.hxx"

#include <map>
#include <string>

#include <util/StringUtil.hxx>
//--------------------------------------------------------------------
#define PTR_SAFE_CALL_RETURN_BOOL(ptr, funcCall) \
   do \
   { \
   bool re = false; \
   if (ptr) \
      { \
      re = ptr->funcCall; \
      } \
      return re;\
   }while(0)

#define PTR_SAFE_CALL(ptr, funcCall) \
   do \
   { \
   if (ptr) \
      { \
      ptr->funcCall; \
      } \
    }while(0)
//--------------------------------------------------------------------
namespace LNpPluginLib
{
   template<class Wrapper>
   class NPObjectWrapper
      :public NPObject
   {
   public:
      NPObjectWrapper(NPP instance, const char* className)
         :mInstance(instance)
         ,mErrorCode(0)
         ,mClassName(LNpPluginLib::StringUtil::getNotNullString(className))
      {
         init();
      }

      virtual ~NPObjectWrapper()
      {}

   public:
      static NPObject* Allocate(NPP npp, NPClass *npclass)
      {
         Wrapper* obj = new Wrapper(npp);
         return obj;
      }

      static void Deallocate(NPObject *npobj)
      {
         delete static_cast<Wrapper*>(npobj);
      }

      static void Invalidate(NPObject *npobj)
      {
         Wrapper* obj = static_cast<Wrapper*>(npobj);
         PTR_SAFE_CALL(obj, invalidate());
      }

      static bool HasMethod(NPObject *npobj, NPIdentifier name)
      {
         bool result = false;
         Wrapper* obj = static_cast<Wrapper*>(npobj);
         PTR_SAFE_CALL_RETURN_BOOL(obj, hasMethod(name));
      }

      static bool Invoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t num_args, NPVariant *result)
      {
         Wrapper* obj = static_cast<Wrapper*>(npobj);
         PTR_SAFE_CALL_RETURN_BOOL(obj, invoke(name, args, num_args, result));
      }

      static bool InvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
      {
         Wrapper* obj = static_cast<Wrapper*>(npobj);
         PTR_SAFE_CALL_RETURN_BOOL(obj, invokeDefault(args, argCount, result));
      }

      static bool HasProperty(NPObject *npobj, NPIdentifier name)
      {
         Wrapper* obj = static_cast<Wrapper*>(npobj);
         PTR_SAFE_CALL_RETURN_BOOL(obj, hasProperty(name));
      }

      static bool GetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
      {
         Wrapper* obj = static_cast<Wrapper*>(npobj);
         PTR_SAFE_CALL_RETURN_BOOL(obj, getProperty(name, result));
      }

      static bool SetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
      {
         Wrapper* obj = static_cast<Wrapper*>(npobj);
         PTR_SAFE_CALL_RETURN_BOOL(obj, setProperty(name, value));
      }

      static bool RemoveProperty(NPObject *npobj, NPIdentifier name)
      {
         Wrapper* obj = static_cast<Wrapper*>(npobj);
         PTR_SAFE_CALL_RETURN_BOOL(obj, removeProperty(name));
      }

   public:
      virtual void invalidate()
      {
      }

      virtual bool invokeDefault(const NPVariant *args,
         uint32_t argCount,
         NPVariant *result)
      {
         return false;
      }

      virtual bool hasMethod(NPIdentifier name)
      {
         return mMethodIdentifiers.hasValue(name, IdentifierDict::FT_METHOD);
      }

      virtual bool invoke(NPIdentifier name, const NPVariant *args, uint32_t num_args, NPVariant *result)
      {
         return mMethodIdentifiers.call(name, args, num_args, result);
      }

      virtual bool hasProperty(NPIdentifier name)
      {
         return (mMethodIdentifiers.hasValue(name, IdentifierDict::FT_GET_PROPERTY) ||
            mMethodIdentifiers.hasValue(name, IdentifierDict::FT_SET_PROPERTY));
      }

      virtual bool getProperty(NPIdentifier name, NPVariant *result)
      {
         return mMethodIdentifiers.call(name, result);
      }

      virtual bool setProperty(NPIdentifier name, const NPVariant *value)
      {
         return mMethodIdentifiers.call(name, value);
      }

      virtual bool removeProperty(NPIdentifier name)
      {
         mMethodIdentifiers.remove(name, IdentifierDict::FT_GET_PROPERTY);
         mMethodIdentifiers.remove(name, IdentifierDict::FT_SET_PROPERTY);
         return true;
      }

   public:
      void setErrorCode(const long errorCode)
      {
         mErrorCode = errorCode;
      }

   private:
      void isEqual(const NPVariant *args, uint32_t num_args, NPVariant *result)
      {
         bool value = false;

         if (NPVARIANT_IS_OBJECT(args[0]))
         {
            NPObject* pThis = static_cast<NPObject*>(this);
            NPObject* pObj = NPVARIANT_TO_OBJECT(args[0]);

            value = (pThis && pObj && pThis->_class == pObj->_class);
         }

         BOOLEAN_TO_NPVARIANT(value, *result);
      }

      void getLastError(const NPVariant *args, uint32_t num_args, NPVariant *result)
      {
         INT32_TO_NPVARIANT(mErrorCode, *result);
      }

      void getClassName(NPVariant *result)
      {
         STRING_TO_NPVARIANT_BY_MALLOC(mClassName, result);
      }

      void init()
      {
         PLUGIN_ADD_METHOD(mMethodIdentifiers, "IsEqual", this, &NPObjectWrapper::isEqual);
         PLUGIN_ADD_METHOD(mMethodIdentifiers, "GetLastError", this, &NPObjectWrapper::getLastError);

         PLUGIN_ADD_GET_PROPERTY(mMethodIdentifiers, "ClassName", this, &NPObjectWrapper::getClassName);
      }

   protected:
      NPP mInstance;
      IdentifierDict mMethodIdentifiers;
      long mErrorCode;
      std::string mClassName;

   private:
      NPObjectWrapper(const NPObjectWrapper&);
      NPObjectWrapper& operator=(const NPObjectWrapper);
   };

   template<class Wrapper>
   NPClass* GetNPClass()
   {
      static NPClass pluginClass =
      {
         NP_CLASS_STRUCT_VERSION,
         NPObjectWrapper<Wrapper>::Allocate,
         NPObjectWrapper<Wrapper>::Deallocate,
         NPObjectWrapper<Wrapper>::Invalidate,
         NPObjectWrapper<Wrapper>::HasMethod,
         NPObjectWrapper<Wrapper>::Invoke,
         NPObjectWrapper<Wrapper>::InvokeDefault,
         NPObjectWrapper<Wrapper>::HasProperty,
         NPObjectWrapper<Wrapper>::GetProperty,
         NPObjectWrapper<Wrapper>::SetProperty,
         NPObjectWrapper<Wrapper>::RemoveProperty,
      };

      return &pluginClass;
   }
} //namespace LNpPluginLib


#endif //#ifndef NPSEEDPLUGIN_NPOBJECTWRAPPER_HXX
