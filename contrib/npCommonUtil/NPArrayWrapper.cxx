#include "NPArrayWrapper.hxx"
#include "NPVariantHelper.hxx"

#include <nscore.h>
#include <cassert>
#include <iostream>

using namespace LNpPluginLib;
//-------------------------------------------------------
//public constructor
NPArrayWrapper::NPArrayWrapper(NPP nPP, const NPVariant* value)
:mNpp(nPP)
,mArrayObj(nsnull)
,mLength(0)
,mValid(false)
{
   init(nPP, value);
}
//-------------------------------------------------------
//public
NPVariant NPArrayWrapper::operator[](int index) const
{
   NPVariant varResult;
   NULL_TO_NPVARIANT(varResult);

   if (index >= 0 && index < mLength && mArrayObj)
   {
      NPIdentifier indexIden = NPN_GetIntIdentifier(index);
      if (indexIden)
      {
         bool result = NPN_GetProperty(mNpp, mArrayObj, indexIden, &varResult);
         assert(result && "NPArrayWrapper NPN_GetProperty failed!");
      }
   }

   return varResult;
}
//-------------------------------------------------------
//private
void NPArrayWrapper::init(NPP nPP, const NPVariant* value)
{
   if (nPP != nsnull && value != nsnull && NPVARIANT_IS_OBJECT(*value))
   {
      mArrayObj = NPVARIANT_TO_OBJECT(*value);
      if (mArrayObj != nsnull)
      {
         NPIdentifier lengthIden = NPN_GetStringIdentifier("length");
         if (nsnull != lengthIden)
         {
            NPVariant lengthVar;
            bool result = NPN_GetProperty(nPP, mArrayObj, lengthIden, &lengthVar);
            if (result && (NPVariantType_Void != lengthVar.type) && (NPVariantType_Null != lengthVar.type))
            {
               mLength = (int32_t)NPVarObjWrapper(lengthVar);
               mValid = true;
            }
         }
      }
   }
}
//-------------------------------------------------------
