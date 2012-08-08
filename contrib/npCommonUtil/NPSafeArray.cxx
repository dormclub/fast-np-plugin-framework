#include "NPSafeArray.hxx"
#include "NPVariantHelper.hxx"

#include <util/StlUtil.hxx>
//--------------------------------------------------------------------
using namespace LNpPluginLib;
//--------------------------------------------------------------------
//public
void NPSafeArray::add(bool value)
{
   NPVariant var;
   BOOLEAN_TO_NPVARIANT(value, var);
   mVarArray.push_back(var);
}

//public
void NPSafeArray::add(long value)
{
   NPVariant var;
   INT32_TO_NPVARIANT(value, var);
   mVarArray.push_back(var);
}

//public
void NPSafeArray::add(double value)
{
   NPVariant var;
   DOUBLE_TO_NPVARIANT(value, var);
   mVarArray.push_back(var);
}

//public
void NPSafeArray::add(const std::string& value)
{
   add(value.c_str());
}

//public
void NPSafeArray::add(const char* value)
{
   NPVariant var;
   if (NULL != value)
   {
      STRINGZ_TO_NPVARIANT(value, var);
   }
   else
   {
      NULL_TO_NPVARIANT(var);
   }

   mVarArray.push_back(var);
}

//public
void NPSafeArray::add(NPObject* value)
{
   NPVariant var;
   if (nsnull != value)
   {
      OBJECT_TO_NPVARIANT_BY_RETAIN(value, &var);
   }
   else
   {
      NULL_TO_NPVARIANT(var);
   }

   mVarArray.push_back(var);
}

//public
unsigned int NPSafeArray::size() const
{
   return mVarArray.size();
}

//public
bool NPSafeArray::isEmpty() const
{
   return mVarArray.empty();
}

//public
void NPSafeArray::clear()
{
   LNpPluginLib::StlUtil::ClearContainer(mVarArray);
}
//--------------------------------------------------------------------

