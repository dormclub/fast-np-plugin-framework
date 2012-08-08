#include "IdentifierDict.hxx"

#include <cassert>
//-----------------------------------------------------------------
using namespace LNpPluginLib;
//-----------------------------------------------------------------
template<typename FuncPtrsType, typename FuncPtrType>
void IdentifierDict::innerAdd(FuncPtrsType& funcPtrs, NPIdentifier identifier, FuncPtrType funcPtr)
{
   typename FuncPtrsType::iterator found = funcPtrs.find(identifier);
   if (found != funcPtrs.end())
   {
      funcPtrs.erase(found);
   }
   std::pair<typename FuncPtrsType::iterator, bool> ret = funcPtrs.insert(typename FuncPtrsType::value_type(identifier, funcPtr));
   assert(ret.second);
}

void IdentifierDict::add(const std::string& identifierName, MethodDelegatePtrType funcPtr)
{
   if (!identifierName.empty())
   {
      NPIdentifier identifier = NPN_GetStringIdentifier(identifierName.c_str());
      if (identifier)
      {
         innerAdd(mIdentifierMethodFuncPtrs, identifier, funcPtr);
      }
   }
}

void IdentifierDict::add(const std::string& identifierName, GetterDelegatePtrType funcPtr)
{
   if (!identifierName.empty())
   {
      NPIdentifier identifier = NPN_GetStringIdentifier(identifierName.c_str());
      if (identifier)
      {
         innerAdd(mIdentifierGetFuncPtrs, identifier, funcPtr);
      }
   }
}

void IdentifierDict::add(const std::string& identifierName, SetterDelegatePtrType funcPtr)
{
   if (!identifierName.empty())
   {
      NPIdentifier identifier = NPN_GetStringIdentifier(identifierName.c_str());
      if (identifier)
      {
         innerAdd(mIdentifierSetFuncPtrs, identifier, funcPtr);
      }
   }
}

void IdentifierDict::remove(const NPIdentifier identifier, FuncType type)
{
   if (identifier)
   {
      switch(type)
      {
      case FT_METHOD:
         mIdentifierMethodFuncPtrs.erase(identifier);
         break;
      case FT_GET_PROPERTY:
         mIdentifierGetFuncPtrs.erase(identifier);
         break;
      case FT_SET_PROPERTY:
         mIdentifierSetFuncPtrs.erase(identifier);
         break;
      }
   }
}

bool IdentifierDict::hasValue(const NPIdentifier identifier, FuncType type)
{
   bool result = false;

   if (identifier)
   {
      switch(type)
      {
      case FT_METHOD:
         result = (mIdentifierMethodFuncPtrs.find(identifier) != mIdentifierMethodFuncPtrs.end());
         break;
      case FT_GET_PROPERTY:
         result = (mIdentifierGetFuncPtrs.find(identifier) != mIdentifierGetFuncPtrs.end());
         break;
      case FT_SET_PROPERTY:
         result = (mIdentifierSetFuncPtrs.find(identifier) != mIdentifierSetFuncPtrs.end());
         break;
      }
   }

   return result;
}

bool IdentifierDict::call(const NPIdentifier identifier, const NPVariant* variant, uint32_t nCount, NPVariant * result)
{
   bool ret = false;

   if (identifier)
   {
      IdentifierMethodFuncPtrMapType::iterator found = mIdentifierMethodFuncPtrs.find(identifier);
      if (found != mIdentifierMethodFuncPtrs.end())
      {
         (*(found->second))(variant, nCount, result);
         ret = true;
      }
   }

   return ret;
}

bool IdentifierDict::call(const NPIdentifier identifier, NPVariant * result)
{
   bool ret = false;

   if (identifier)
   {
      IdentifierGetterFuncPtrMapType::iterator found = mIdentifierGetFuncPtrs.find(identifier);
      if (found != mIdentifierGetFuncPtrs.end())
      {
         (*(found->second))(result);
         ret = true;
      }
   }

   return ret;
}

bool IdentifierDict::call(const NPIdentifier identifier, const NPVariant *variant)
{
   bool ret = false;

   if (identifier)
   {
      IdentifierSetterFuncPtrMapType::iterator found = mIdentifierSetFuncPtrs.find(identifier);
      if (found != mIdentifierSetFuncPtrs.end())
      {
         (*(found->second))(variant);
         ret = true;
      }
   }

   return ret;
}
