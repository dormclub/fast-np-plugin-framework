#ifndef NP_RUNTIME_IDENTIFIER_DICT_H_
#define NP_RUNTIME_IDENTIFIER_DICT_H_

#include <npapi.h>
#include <npruntime.h>

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <delegate/delegate.hxx>
//--------------------------------------------------------
namespace LNpPluginLib
{
   class IdentifierDict
   {
   public:
      enum FuncType
      {
         FT_METHOD = 0,
         FT_GET_PROPERTY,
         FT_SET_PROPERTY
      };

   public:
      typedef LNpPluginLib::Delegate<void (const NPVariant*, uint32_t, NPVariant*)> MethodDelegateType;
      typedef LNpPluginLib::Delegate<void (NPVariant *)> GetterDelegateType;
      typedef LNpPluginLib::Delegate<void (const NPVariant *)> SetterDelegateType;

      typedef boost::shared_ptr<MethodDelegateType> MethodDelegatePtrType;
      typedef boost::shared_ptr<GetterDelegateType> GetterDelegatePtrType;
      typedef boost::shared_ptr<SetterDelegateType> SetterDelegatePtrType;

      typedef std::map<NPIdentifier, MethodDelegatePtrType> IdentifierMethodFuncPtrMapType;
      typedef std::map<NPIdentifier, GetterDelegatePtrType> IdentifierGetterFuncPtrMapType;
      typedef std::map<NPIdentifier, SetterDelegatePtrType> IdentifierSetterFuncPtrMapType;

   public:
      IdentifierDict(void)
      {
      }

      ~IdentifierDict(void)
      {
         clear();
      }

   public:
      void clear()
      {
         mIdentifierMethodFuncPtrs.clear();
         mIdentifierGetFuncPtrs.clear();
         mIdentifierSetFuncPtrs.clear();
      }

      bool hasValue(const NPIdentifier identifier, FuncType type);
      void remove(const NPIdentifier identifier, FuncType type);

      bool call(const NPIdentifier identifier, const NPVariant* variant, uint32_t nCount, NPVariant * result);
      bool call(const NPIdentifier identifier, NPVariant * result);
      bool call(const NPIdentifier identifier, const NPVariant *variant);

      void add(const std::string& identifierName, MethodDelegatePtrType funcPtr);
      void add(const std::string& identifierName, GetterDelegatePtrType funcPtr);
      void add(const std::string& identifierName, SetterDelegatePtrType funcPtr);

   private:
      template<typename FuncPtrsType, typename FuncPtrType>
      void innerAdd(FuncPtrsType& funcPtrs, NPIdentifier identifier, FuncPtrType funcPtr);

   private:
      IdentifierMethodFuncPtrMapType mIdentifierMethodFuncPtrs;
      IdentifierGetterFuncPtrMapType mIdentifierGetFuncPtrs;
      IdentifierSetterFuncPtrMapType mIdentifierSetFuncPtrs;
   };

   //------------------------------------------------------- IDENTIFIER MACRO
#define PLUGIN_ADD_METHOD_EX(methodName, senderPtr, methodDelegateFunc) \
   do \
   { \
   if (methodName && methodName[0] && methodDelegateFunc) \
   { \
   mMethodIdentifiers.add(methodName, IdentifierDict::MethodDelegatePtrType(new IdentifierDict::MethodDelegateType(std::make_pair(senderPtr, methodDelegateFunc)))); \
   } \
   } while(0)

#define PLUGIN_ADD_GET_PROPERTY_EX(propertyName, senderPtr, propertyDelegateFunc) \
   do \
   { \
   if (propertyName && propertyName[0] && propertyDelegateFunc) \
   { \
   mMethodIdentifiers.add(propertyName, IdentifierDict::GetterDelegatePtrType(new IdentifierDict::GetterDelegateType(std::make_pair(senderPtr, propertyDelegateFunc)))); \
   } \
   } while(0)

#define PLUGIN_ADD_SET_PROPERTY_EX(propertyName, senderPtr, propertyDelegateFunc) \
   do \
   { \
   if (propertyName && propertyName[0] && propertyDelegateFunc) \
   { \
   mMethodIdentifiers.add(propertyName, IdentifierDict::SetterDelegatePtrType(new IdentifierDict::SetterDelegateType(std::make_pair(senderPtr, propertyDelegateFunc)))); \
   } \
   } while(0)

#define PLUGIN_ADD_METHOD(methodIdentifiers, methodName, senderPtr, methodDelegateFunc) \
   do \
   { \
   if (methodName && methodName[0] && methodDelegateFunc) \
   { \
   methodIdentifiers.add(methodName, IdentifierDict::MethodDelegatePtrType(new IdentifierDict::MethodDelegateType(std::make_pair(senderPtr, methodDelegateFunc)))); \
   } \
   } while(0)

#define PLUGIN_ADD_GET_PROPERTY(proIdentifiers, propertyName, senderPtr, propertyDelegateFunc) \
   do \
   { \
   if (propertyName && propertyName[0] && propertyDelegateFunc) \
   { \
   proIdentifiers.add(propertyName, IdentifierDict::GetterDelegatePtrType(new IdentifierDict::GetterDelegateType(std::make_pair(senderPtr, propertyDelegateFunc)))); \
   } \
   } while(0)

#define PLUGIN_ADD_SET_PROPERTY(proIdentifiers, propertyName, senderPtr, propertyDelegateFunc) \
   do \
   { \
   if (propertyName && propertyName[0] && propertyDelegateFunc) \
   { \
   proIdentifiers.add(propertyName, IdentifierDict::SetterDelegatePtrType(new IdentifierDict::SetterDelegateType(std::make_pair(senderPtr, propertyDelegateFunc)))); \
   } \
   } while(0)


}//namespace LNpPluginLib

#endif //#ifndef NP_RUNTIME_IDENTIFIER_DICT_H_


