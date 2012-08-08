#ifndef NPSEEDPLUGIN_NPSEEDCOMMON_HXX
#define NPSEEDPLUGIN_NPSEEDCOMMON_HXX

#include <npapi.h>

#include <string>
//-----------------------------------------------------------------
class NPSeedCommon
{
public:
   static void getUserAgentInfo(NPP npp, const std::string seedVersion, std::string& userAgentInfo);
   static void getAccountId(std::string& accountId);

private:
   static void extractBrowserVersion(const char* userAgentInfo, std::string& versionInfo);
   static void subBrowserVersion(std::string userAgentInfoStr, std::string::size_type pos, std::string& userAgentInfo);
private:
   NPSeedCommon();
   NPSeedCommon(const NPSeedCommon&);
   NPSeedCommon& operator=(const NPSeedCommon&);
};

#endif //#ifndef NPSEEDPLUGIN_NPSEEDCOMMON_HXX