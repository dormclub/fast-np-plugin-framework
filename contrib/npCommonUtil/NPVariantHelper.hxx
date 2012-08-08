#ifndef NPSEEDPLUGIN_NPVAROBJWRAPPER_HXX
#define NPSEEDPLUGIN_NPVAROBJWRAPPER_HXX

#include <npapi.h>
#include <npruntime.h>
#include <nscore.h>

#include <util/StlUtil.hxx>
#include <util/StringUtil.hxx>

#include <boost/shared_array.hpp>
//----------------------------------------------------------
namespace LNpPluginLib
{
   class NPVariantHelper
   {
   public:
      static void assign(const std::string& value, NPVariant* newValue);
      template<typename type>
      static void assign(NPP npp, NPVariant* newValue);
      static bool getValueByName(NPP npp, const NPVariant& var, NPVariant& value);
   };

   //-----------------------------------------------------------------------
   class NPVarObjWrapper
   {
   private:
      enum NPVarObjVariantType
      {
         NPVarObjVariantType_Empty,
         NPVarObjVariantType_Int32,
         NPVarObjVariantType_Double,
         NPVarObjVariantType_String,
         NPVarObjVariantType_Object,
      };

      struct NPVarObjVariant
      {
         NPVarObjVariant()
            :type(NPVarObjVariantType_Empty)
         {
            ::memset(&value, 0, sizeof(value));
         }

         NPVarObjVariantType type;
         union {
            int32_t intValue;
            double doubleValue;
            NPObject* objectValue;
         }value;

         boost::shared_array<char> stringValArray;
      };

   public:
      NPVarObjWrapper()
      {
      }


      NPVarObjWrapper(const NPVarObjWrapper&);
      NPVarObjWrapper& operator=(const NPVarObjWrapper&);

      explicit NPVarObjWrapper(const NPVariant* value);
      explicit NPVarObjWrapper(const NPVariant& value);

      ~NPVarObjWrapper();

   public:
      operator NPObject*() const;

      bool isObject();

      const char* c_str();

      std::string str();

      operator double() const;

      operator int32_t() const;

      operator long long() const;

      operator unsigned long long() const
      {
         return operator long long();
      }

      operator long() const
      {
         return operator int32_t();
      }

      NPVarObjWrapper& operator=(const NPVariant* value);

      void release();

   private:
      void init(const NPVariant& var);
      int32_t strToInt32() const;
      double strToDouble() const;
      bool isDigit(const char* strValue) const;
      void toString();

      void clone(const NPVarObjWrapper& obj);

   private:
      NPVarObjVariant mValue;
   };

   //---------------------------------------------------
#define STRING_TO_NPVARIANT_BY_MALLOC(value, newValue) \
   do \
   { \
   NPVariantHelper::assign(value, newValue); \
   } while(0)

#define OBJECT_TO_NPVARIANT_BY_RETAIN(value, newValue) \
   do \
   { \
   if (newValue != nsnull) \
   { \
   if (value != nsnull) \
   { \
   OBJECT_TO_NPVARIANT(NPN_RetainObject(value), *newValue); \
   } \
   else \
   { \
   NULL_TO_NPVARIANT(*newValue); \
   } \
   } \
   } while(0)

#define NPVARIANT_TO_NEW_STRING(value, newValue) \
   do \
   { \
   NPString npString = NPVARIANT_TO_STRING(value); \
   if (npString.utf8length > 0) \
   { \
   newValue = npString.utf8characters; \
   newValue.resize(npString.utf8length); \
   } \
   } while(0)

#define RELEASE_NP_OBJECT(obj) \
   do \
   { \
   if (nsnull != obj && obj->referenceCount != 0) \
   { \
   NPN_ReleaseObject(obj); \
   obj = nsnull; \
   } \
   }while(0)
}//namespace LNpPluginLib

#endif //#ifndef NPSEEDPLUGIN_NPVAROBJWRAPPER_HXX
