#pragma once
#include "UObject/UnrealType.h"
#include <string>
#include <vector>
#include <sstream>

extern TArray<FString> g_strCfgArrays;

class GStrUtil
{
public:
	template <class T>
	static T StringParse(const FString& s, T defaultValue)
	{
		if (s.IsEmpty())
		{
			return defaultValue;
		}

		T value;
		std::istringstream iss(TCHAR_TO_UTF8(*s));
		iss >> value;
		return value;
	}
	
	static bool StringParse(const FString& s, bool defaultValue)
	{
		if (s.IsEmpty())
		{
			return defaultValue;
		}
		bool value;
		std::istringstream iss(TCHAR_TO_UTF8(*s));
		iss>> value;
		if(!value)
		{
			iss.clear();
			iss.str(TCHAR_TO_UTF8(*s.ToLower()));
			iss >> std::boolalpha >> value;
		}
		return value;
	}


	template <class T>
	static void ArrayParse(const FString& value, TArray<T>& result)
	{
		g_strCfgArrays.Empty();
		result.Empty();

		if (!value.IsEmpty())
		{
			value.ParseIntoArray(g_strCfgArrays, TEXT("|"), false);

			for (auto& v : g_strCfgArrays)
			{
				result.Add(StringParse<T>(v, 0));
			}
		}
	}


	template< class T>
	static bool ExtractNumericList(const FString& value, std::vector<T>& result, std::vector<T>* defaultVal)
	{
		g_strCfgArrays.Empty();
		result.clear();

		if (value.IsEmpty())
		{
			if (defaultVal != nullptr)
			{
				result.insert(result.end(), defaultVal->begin(), defaultVal->end());
			}
		}
		else
		{
			value.ParseIntoArray(g_strCfgArrays, TEXT("|"), false);

			for (auto & v : g_strCfgArrays)
			{
				result.push_back(StringParse<T>(v, 0));
			}
		}

		return !result.empty();
	}

	static bool ExtractString(const FString& value, std::string& str, const std::string& defaultVal)
	{
		if (value.IsEmpty())
		{
			str = defaultVal;
		}

		str = TCHAR_TO_UTF8(*value);

		return true;
	}

	static bool ExtractStringList(const FString& value, std::vector<std::string>& result, std::vector<std::string>* defaultVal)
	{
		g_strCfgArrays.Empty();
		result.clear();
		if (value.IsEmpty())
		{
			if (defaultVal != nullptr)
			{
				result.insert(result.end(), defaultVal->begin(), defaultVal->end());
			}
		}
		else
		{
			value.ParseIntoArray(g_strCfgArrays, TEXT("|"), false);
			for (auto & v : g_strCfgArrays)
			{
				result.emplace_back();
				ExtractString(v, result.back(), "");
			}
		}
		return true;
	}
};