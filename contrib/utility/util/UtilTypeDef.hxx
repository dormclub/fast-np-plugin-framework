#ifndef LNPLUGINLIB_UTILTYPEDEF_HXX
#define LNPLUGINLIB_UTILTYPEDEF_HXX

#include <boost/shared_ptr.hpp>

#ifdef USING_USTL
# include <ustl.h>
using namespace ustl;
#else
# include <string>
using namespace std;
#endif

#include <set>

namespace LNpPluginLib
{
   typedef boost::shared_ptr<string> SharedString;
   typedef set<SharedString> SharedStringSet;
}

#endif//#ifndef LNPLUGINLIB_UTILTYPEDEF_HXX