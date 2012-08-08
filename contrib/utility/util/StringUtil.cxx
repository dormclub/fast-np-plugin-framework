#include <util/StringUtil.hxx>

using namespace LNpPluginLib;

const string StringUtil::sEmpty = "";

#if defined(_WIN32) || defined(WIN32)
const std::wstring StringUtil::sWEmpty = L"";
#endif

const SharedString StringUtil::sEmptySharedString(new string(StringUtil::sEmpty));