#ifndef LNPLUGINLIB_STRINGUTIL_HXX
#define LNPLUGINLIB_STRINGUTIL_HXX

#include <util/UtilTypeDef.hxx>

#ifdef USING_USTL
# include <ustl.h>
using namespace ustl;
#else
# include <string>
# include <list>
# include <deque>
# include <vector>
using namespace std;
#endif

namespace LNpPluginLib
{
   class StringUtil
   {
   public:
      static const string sEmpty;
      static const SharedString sEmptySharedString;
#if defined(_WIN32) || defined(WIN32)
      static const std::wstring sWEmpty;
#endif

      static const char* getNotNullString(const char* strValue)
      {
         return (strValue?strValue:sEmpty.c_str());
      }


      template<typename StringPtrType>
      static bool NullOrEmpty(StringPtrType strPtr)
      {
         return !(strPtr && strPtr[0]);
      }

      template<typename StringPtrType>
      static bool NotNullOrEmpty(StringPtrType strPtr)
      {
         return (strPtr && strPtr[0]);
      }

      template<typename StringType>
      static void Trim(StringType& toTrim)
      {
         TrimRight(toTrim);
         TrimLeft(toTrim);
      }

      template<typename StringType>
      static void TrimLeft(StringType& toTrim)
      {
         while (::isspace(toTrim[0]))
            toTrim.erase(toTrim.begin(), toTrim.begin() + 1);
      }

      template<typename StringType>
      static void TrimRight(StringType& toTrim)
      {
         while (::isspace(toTrim[toTrim.length() - 1]))
            toTrim.erase(toTrim.length() - 1);
      }

      template<typename StringType>
      static void replace(StringType& strBig, const StringType& strSrc, const StringType& strDst)
      {
         typedef typename StringType::size_type size_type;
         size_type pos = 0;
         size_type srcLen = strSrc.size();
         size_type dstLen = strDst.size();

         while((pos=strBig.find(strSrc, pos)) != StringType::npos)
         {
            strBig.replace(pos, srcLen, strDst);
            pos += dstLen;
         }
      }

      static string makeCSV(const list<string>& source)
      {
         return internalMakeCSV(source);
      }

      static string makeCSV(const vector<string>& source)
      {
         return internalMakeCSV(source);
      }

      static string makeCSV(const deque<string>& source)
      {
         return internalMakeCSV(source);
      }

      static void partitionCSV(const char* source, list<string>& result)
      {
         internalPartitionCSV(source, result);
      }

      static void partitionCSV(const char* source, vector<string>& result)
      {
         internalPartitionCSV(source, result);
      }

      static void partitionCSV(const char* source, deque<string>& result)
      {
         internalPartitionCSV(source, result);
      }

      template<class InputIterator>
      static void initString(SharedString& str, InputIterator start, InputIterator end)
      {
         if (StringUtil::sEmptySharedString == str)
         {
            str.reset(new string(start, end));
         }
         else
         {
            str->assign(start, end);
         }
      }
      static void initString(SharedString& sharStr, const char* str)
      {
         if (StringUtil::sEmptySharedString == sharStr)
         {
            sharStr.reset(new string(str));
         }
         else
         {
            sharStr->assign(str);
         }
      }

      template <typename SequenceContainerType>
      static string internalMakeCSV(const SequenceContainerType& source)
      {
         string sink(LNpPluginLib::StringUtil::sEmpty);
         typename SequenceContainerType::const_iterator i;
         for (i = source.begin();  i != source.end(); ++i)
         {
            if (i != source.begin())
            {
               sink += ',';
            }

            sink += *i;
         }

         return sink;
      }

      template <typename SequenceContainerType>
      static void internalPartitionCSV(const char* source, SequenceContainerType& result)
      {
         if (source)
         {
            string input(source);
            if (!input.empty())
            {
               string::size_type pos = 0;
               string::size_type next = 0;
               do
               {
                  next = input.find(',', pos);
                  result.push_back(input.substr(pos, next-pos));
                  pos = next + 1;
               } while (next != string::npos);
            }
         }
      }
   };

} // namespace LNpPluginLib

#endif // #ifndef LNPLUGINLIB_STRINGUTIL_HXX