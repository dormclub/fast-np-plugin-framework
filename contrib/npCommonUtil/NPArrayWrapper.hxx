#ifndef NPSEEDPLUGIN_NPARRAYWRAPPER_HXX
#define NPSEEDPLUGIN_NPARRAYWRAPPER_HXX

#include <npapi.h>
#include <npruntime.h>
//-------------------------------------------------------
namespace LNpPluginLib
{
   class NPArrayWrapper
   {
   public:
      NPArrayWrapper(NPP nPP, const NPVariant* value);

   public:
      int length() const { return mLength; }
      NPVariant operator[](int index) const;
      bool isValid() const { return mValid; };

   private:
      void init(NPP nPP, const NPVariant* value);

   private:
      NPP mNpp;
      NPObject* mArrayObj;
      unsigned int mLength;
      bool mValid;
   };

}//namespace LNpPluginLib

#endif //#ifndef NPSEEDPLUGIN_NPARRAYWRAPPER_HXX
