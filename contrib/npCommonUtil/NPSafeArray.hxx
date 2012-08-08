#ifndef NPSEEDPLUGIN_NPSAFEARRAY_HXX
#define NPSEEDPLUGIN_NPSAFEARRAY_HXX

#include <npapi.h>
#include <npruntime.h>
#include <nscore.h>

#include <vector>
//----------------------------------------------------------
namespace LNpPluginLib
{
   class NPSafeArray
   {
   private:
      typedef std::vector<NPVariant> VariantArrayType;

   public:
      ~NPSafeArray()
      {
         clear();
      }

   public:
      void add(bool value);
      void add(long value);
      void add(double value);
      void add(const std::string& value);
      void add(const char* value);
      void add(NPObject* value);

      void clear();

      unsigned int size() const;
      bool isEmpty() const;

      operator const NPVariant *()
      {
         if (!mVarArray.empty())
         {
            return &mVarArray[0];
         }
         else
         {
            return nsnull;
         }
      }

   private:
      VariantArrayType mVarArray;
   };

}//namespace LNpPluginLib

#endif //#ifndef NPSEEDPLUGIN_NPSAFEARRAY_HXX
