#ifndef LNPLUGINLIB_STLUTIL_HXX
#define LNPLUGINLIB_STLUTIL_HXX

#include <util/StringUtil.hxx>

#ifdef USING_USTL
# include <ustl.h>
using namespace ustl;
#else
# include <set>
# include <map>
using namespace std;
#endif

namespace LNpPluginLib
{
   class StlUtil
   {
   public:
      template<typename T>
      static void ClearContainer(T& container)
      {
         T().swap(container);
      }

      template<class ContainerType>
      static typename ContainerType::iterator Erase(ContainerType& container, typename ContainerType::iterator& eraseIterator)
      {
         if (eraseIterator != container.end())
         {
            eraseIterator = container.erase(eraseIterator);
         }
         return eraseIterator;
      }

      template<class ContainerType>
      static typename ContainerType::iterator TreeErase(ContainerType& container, typename ContainerType::iterator& eraseIterator)
      {
         if (eraseIterator != container.end())
         {
            typename ContainerType::iterator toDelete = eraseIterator;
            ++eraseIterator;
            container.erase(toDelete);
         }
         return eraseIterator;
      }
#ifdef USING_USTL
      template<class _Kty>
      static void Erase(set<_Kty>& container, typename set<_Kty>::iterator& eraseIterator)
      {
         TreeErase(container, eraseIterator);
      }

      template<class _Kty, class _Ty>
      static void Erase(map<_Kty, _Ty>& container, typename map<_Kty, _Ty>::iterator& eraseIterator)
      {
         TreeErase(container, eraseIterator);
      }
#else
      template<class _Kty, class _Pr, class _Alloc>
      static typename std::set<_Kty, _Pr, _Alloc>::iterator Erase(std::set<_Kty, _Pr, _Alloc>& container, typename std::set<_Kty, _Pr, _Alloc>::iterator& eraseIterator)
      {
         return TreeErase(container, eraseIterator);
      }

      template<class _Kty, class _Ty, class _Pr, class _Alloc>
      static typename std::map<_Kty, _Ty, _Pr, _Alloc>::iterator Erase(std::map<_Kty, _Ty, _Pr, _Alloc>& container, typename std::map<_Kty, _Ty, _Pr, _Alloc>::iterator& eraseIterator)
      {
         return TreeErase(container, eraseIterator);
      }
#endif

   };
}

#endif // #ifndef LNPLUGINLIB_STLUTIL_HXX
