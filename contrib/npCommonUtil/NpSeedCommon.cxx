#include "NpSeedCommon.hxx"

#include "SeedCommon.h"

#include <tutil/util/ClientInfo.hxx>
#include <tutil/util/StringUtil.hxx>
//-----------------------------------------------------------------
//public static
void NPSeedCommon::getUserAgentInfo(NPP npp, const std::string seedVersion, std::string& userAgentInfo)
{
   userAgentInfo = seedVersion;

   const char* userAgent = NPN_UserAgent(npp);
   std::string browserInfo;
   extractBrowserVersion(userAgent, browserInfo);
   std::string osVersion = TUtil::ClientInfo::getOsVersion();
   if (!osVersion.empty() || !browserInfo.empty())
   {
      userAgentInfo += ("(");

      if (!osVersion.empty())
      {
         userAgentInfo += osVersion;
      }

      if (!browserInfo.empty())
      {
         userAgentInfo += "; ";
         userAgentInfo += browserInfo;
      }

      userAgentInfo += (")");
   }
}

//public static
void NPSeedCommon::getAccountId(std::string& accountId)
{
#ifdef WIN32
   std::string uniqueId = TUtil::StringUtil::sEmpty;
   SeedCommon::getMachineUid(uniqueId);
#else
   std::string uniqueId = "temp";
#endif

#ifdef WIN32
   std::string winCurrentUser = TUtil::ClientInfo::getWindowsCurrentUser();
#else
   std::string winCurrentUser = "guset";
#endif
   TUtil::StringUtil::Trim(winCurrentUser);
   winCurrentUser = TUtil::StringUtil::encodeUrl(winCurrentUser.c_str());

   accountId = uniqueId + "." + winCurrentUser;
}
//-----------------------------------------------------------------
//private
void NPSeedCommon::extractBrowserVersion(const char* userAgentInfo, std::string& versionInfo)
{
   versionInfo = TUtil::StringUtil::sEmpty;

   if (TUtil::StringUtil::NotNullOrEmpty(userAgentInfo))
   {
      std::string userAgentInfoStr(userAgentInfo);

      std::string::size_type startPos = userAgentInfoStr.find("Firefox");
      if (std::string::npos != startPos)
      {
         subBrowserVersion(userAgentInfoStr, startPos, versionInfo);
      }
      else if (std::string::npos != (startPos = userAgentInfoStr.find("Opera")))
      {
         subBrowserVersion(userAgentInfoStr, startPos, versionInfo);
      }
      else if (std::string::npos != (startPos = userAgentInfoStr.find("Safari")))
      {
         std::string::size_type newPos = userAgentInfoStr.find("Chrome");
         if (std::string::npos != newPos)
         {
            subBrowserVersion(userAgentInfoStr, newPos, versionInfo);
         }
         else
         {
            newPos = userAgentInfoStr.find("Version/");
            if (std::string::npos != newPos)
            {
               userAgentInfoStr = userAgentInfoStr.substr(newPos);
               std::string::size_type diagonalPos = userAgentInfoStr.find_first_of("/");
               if (std::string::npos != (diagonalPos + 1))
               {
                  subBrowserVersion(userAgentInfoStr, diagonalPos + 1, userAgentInfoStr);
                  versionInfo = "Safari/" + userAgentInfoStr;
               }
            }
            else
            {
               subBrowserVersion(userAgentInfoStr, startPos, versionInfo);
            }
         }
      }
   }
}

//private
void NPSeedCommon::subBrowserVersion(std::string userAgentInfoStr,
                                     std::string::size_type pos,
                                     std::string& userAgentInfo)
{
   userAgentInfoStr = userAgentInfoStr.substr(pos);
   std::string::size_type endPos = userAgentInfoStr.find(" ");
   userAgentInfo = userAgentInfoStr.substr(0, endPos);
}
//-----------------------------------------------------------------
