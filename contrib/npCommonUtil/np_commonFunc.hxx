#ifndef NPSEEDPLUGIN_COMMONFUNCT_HXX
#define NPSEEDPLUGIN_COMMONFUNCT_HXX

#include <npruntime.h>
#include <npapi.h>

namespace LNpPluginLib
{
   class IBasePlugin;
}
//--------------------------------------------------------
// nsPlugin helper function : Need to achieve their own plugin
//#ifdef XP_UNIX
//char*
//NPP_GetMIMEDescription(void)  -------------- return mime type
//{
//   return "application/x-npruntime-solarseed-plugin";
//}
//#endif
//
//IBasePlugin* NPP_GetPlugin(NPP pNPInstance) ---- create plugin object
//{
//   return new CPlugin(pNPInstance);
//}
//--------------------------------------------------------
#ifdef XP_UNIX
char* NPP_GetMIMEDescription(void);
#endif

LNpPluginLib::IBasePlugin* NPP_GetPlugin(NPP pNPInstance);
//--------------------------------------------------------
#endif //#ifndef NPSEEDPLUGIN_COMMONFUNCT_HXX