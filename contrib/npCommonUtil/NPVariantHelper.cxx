#include "NPVariantHelper.hxx"
#include "NPObjectWrapper.hxx"

#include <compat.hxx>

#include <cstring>
#include <cassert>
#include <cctype>

const int DOUBLE_STR_SIZE = 25;
const int INT32_STR_SIZE = 12;
//--------------------------------------------------------------------
using namespace LNpPluginLib;
//--------------------------------------------------------------------
void NPVariantHelper::assign(const std::string& value, NPVariant* newValue)
{
   if (newValue)
   {
      char* cValue = reinterpret_cast<char*>(NPN_MemAlloc((uint32)value.length() + 1));
      assert(cValue);
      memcpy(cValue, value.c_str(), value.length());
      cValue[value.length()] = '\0';
      (*newValue).type = NPVariantType_String;
      (*newValue).value.stringValue.utf8length = (uint32_t)value.length();
      (*newValue).value.stringValue.utf8characters = cValue;
   }
}

template<typename type>
void NPVariantHelper::assign(NPP npp, NPVariant* newValue)
{
   if (newValue != nsnull && npp != nsnull)
   {
      OBJECT_TO_NPVARIANT(NPN_CreateObject(npp, GetNPClass<type>()), *newValue);
   }
}

bool NPVariantHelper::getValueByName(NPP npp, const NPVariant& var, NPVariant& value) //obtain javascript variant value by name
{
   bool result = false;

   NULL_TO_NPVARIANT(value);

   if (npp != nsnull && var.type == NPVariantType_String)
   {
      NPObject* windowObj = nsnull;
      NPError npError = NPN_GetValue(npp, NPNVWindowNPObject, &windowObj);
      if (npError == NPERR_NO_ERROR && windowObj != nsnull)
      {
         NPIdentifier flagIdent = NPN_GetStringIdentifier(NPVarObjWrapper(var).c_str());
         if (flagIdent)
         {
            result = NPN_GetProperty(npp, windowObj, flagIdent, &value);
         }
      }
   }

   return result;
}
//--------------------------------------------------------------------
NPVarObjWrapper::NPVarObjWrapper(const NPVariant* value)
{
   if (value != nsnull)
   {
      init(*value);
   }
}

NPVarObjWrapper::NPVarObjWrapper(const NPVariant& value)
{
   init(value);
}


NPVarObjWrapper::NPVarObjWrapper(const NPVarObjWrapper& npVarObj)
{
   clone(npVarObj);
}

NPVarObjWrapper& NPVarObjWrapper::operator=(const NPVarObjWrapper& npVarObj)
{
   clone(npVarObj);
   return (*this);
}

NPVarObjWrapper& NPVarObjWrapper::operator=(const NPVariant* value)
{   
   init(*value);

   return (*this);
}

NPVarObjWrapper::~NPVarObjWrapper()
{
   release();
}

void NPVarObjWrapper::release()
{
   switch (mValue.type)
   {
   case NPVarObjVariantType_Object:
      RELEASE_NP_OBJECT(mValue.value.objectValue);
      break;
   case NPVarObjVariantType_String:
      mValue.stringValArray.reset();
      break;
   }
   mValue.type = NPVarObjVariantType_Empty;
   ::memset(&mValue.value, 0, sizeof(mValue.value));
}

bool NPVarObjWrapper::isObject()
{
   return (mValue.type == NPVarObjVariantType_Object);
}

const char* NPVarObjWrapper::c_str()
{
   toString();
   return LNpPluginLib::StringUtil::getNotNullString(mValue.stringValArray.get());
}

std::string NPVarObjWrapper::str()
{
   return c_str();
}

void NPVarObjWrapper::toString()
{
   if (mValue.stringValArray)
   {
      return;
   }

   switch(mValue.type)
   {
   case NPVarObjVariantType_Double:
      {
         mValue.stringValArray.reset(new char[DOUBLE_STR_SIZE]);
         ::snprintf(mValue.stringValArray.get(), DOUBLE_STR_SIZE, "%f", mValue.value.doubleValue);
      }
      break;
   case NPVarObjVariantType_Int32:
      {
         mValue.stringValArray.reset(new char[INT32_STR_SIZE]);
         ::snprintf(mValue.stringValArray.get(), INT32_STR_SIZE, "%d", mValue.value.intValue);
      }
      break;
   default:
      ;
   }
}

NPVarObjWrapper::operator NPObject*() const
{
   if (mValue.type == NPVarObjVariantType_Object)
   {
      return mValue.value.objectValue;
   }

   return nsnull;
}

NPVarObjWrapper::operator double() const
{
   switch(mValue.type)
   {
   case NPVarObjVariantType_Double:
      return mValue.value.doubleValue;
   case NPVarObjVariantType_String:
      return strToDouble();
   default:
      return 0.0;
   }
}

NPVarObjWrapper::operator int32_t() const
{
   switch(mValue.type)
   {
   case NPVarObjVariantType_Int32:
      return mValue.value.intValue;
   case NPVarObjVariantType_String:
      return strToInt32();
   default:
      return 0;
   }
}

NPVarObjWrapper::operator long long() const
{
   switch(mValue.type)
   {
   case NPVarObjVariantType_Int32:
      return mValue.value.intValue;
   case NPVarObjVariantType_Double:
      return mValue.value.doubleValue;
   case NPVarObjVariantType_String:
      return strToDouble();
   default:
      return 0;
   }
}

//private
void NPVarObjWrapper::init(const NPVariant& var)
{
   release();
   switch(var.type)
   {
   case NPVariantType_Object:
      {
         if (var.value.objectValue)
         {
            mValue.type = NPVarObjVariantType_Object;
            mValue.value.objectValue = NPN_RetainObject(var.value.objectValue);
         }
      }
      break;
   case NPVariantType_String:
      {
         mValue.type = NPVarObjVariantType_String;
         uint32_t strSize = var.value.stringValue.utf8length;
         mValue.stringValArray.reset(new char[strSize + 1]);
         ::memcpy(mValue.stringValArray.get(), var.value.stringValue.utf8characters, strSize);
         mValue.stringValArray[strSize] = '\0';
      }
      break;
   case NPVariantType_Int32:
      {
         mValue.type = NPVarObjVariantType_Int32;
         mValue.value.intValue = var.value.intValue;
      }
      break;
   case NPVariantType_Double:
      {
         mValue.type = NPVarObjVariantType_Double;
         mValue.value.doubleValue = var.value.doubleValue;
      }
      break;
   default:
      ;
   }
}

//private
int32_t NPVarObjWrapper::strToInt32() const
{
   int32_t restult = 0;

   if (isDigit(mValue.stringValArray.get()))
   {
      restult = ::atoi(mValue.stringValArray.get());
   }

   return restult;
}

double NPVarObjWrapper::strToDouble() const
{
   double restult = 0.0;

   if (isDigit(mValue.stringValArray.get()))
   {
      restult = ::atof(mValue.stringValArray.get());
   }

   return restult; 
}

bool NPVarObjWrapper::isDigit(const char* strValue) const
{
   bool rt = true;

   if (NULL != strValue)
   {
      uint32_t size = ::strlen(strValue);
      for (uint32_t i = 0; i < size; ++i)
      {
         if ('.' == strValue[i])
         {
            continue;
         }

         if (!std::isdigit(strValue[i]))
         {
            rt = false;
            break;
         }
      }
   }
   else
   {
      rt = false;
   }

   return rt;
}

void NPVarObjWrapper::clone(const NPVarObjWrapper& obj)
{
   if (&obj != this)
   {
      release();
      mValue.type = obj.mValue.type;
      if (obj.mValue.type == NPVarObjVariantType_Object)
      {
         mValue.value.objectValue = NPN_RetainObject(obj.mValue.value.objectValue);
      }
      else if (obj.mValue.type == NPVarObjVariantType_String)
      {
         mValue.stringValArray = obj.mValue.stringValArray;
      }
      else
      {
         ::memcpy(&mValue.value, &obj.mValue.value, sizeof(mValue.value));
      }
   }
}