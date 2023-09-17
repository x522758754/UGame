#include "Config/ConfigRowParser.h"

#include "Config/GStrUtil.h"

GConfigRowParser::~GConfigRowParser()
{
}

//////////////////////////////////////////////////////////////////////////

TxtConfigRowParser::TxtConfigRowParser()
{

}

TxtConfigRowParser::~TxtConfigRowParser()
{

}

void TxtConfigRowParser::Reset(const FString& str)
{
	m_str = &str;
	//ParseIntoArray();
	m_str->ParseIntoArray(ParserArray, TEXT("\t"), false);
}

void TxtConfigRowParser::Reserve(int Count)
{
	while(ParserArray.Num() < Count)
	{
		ParserArray.Add(FString());
	}
}

int TxtConfigRowParser::GetId()
{
	if (ParserArray.Num() == 0)
	{
		return -1;
	}
	return GStrUtil::StringParse<int>(ParserArray[0], -1);
}

bool TxtConfigRowParser::ParseIntoArray()
{
	const TCHAR* Start = **m_str;
	const TCHAR* pchDelim = TEXT("\t");
	const int32 DelimLength = 1;
	bool First = ParserArray.Num() == 0;
	if (Start && *Start != TEXT('\0'))
	{
		int Count = 0;
		while (const TCHAR* At = FCString::Strstr(Start, pchDelim))
		{
			if (First)
			{
				ParserArray.Emplace(UE_PTRDIFF_TO_INT32(At - Start), Start);
			}
			else
			{
				if (Count >= ParserArray.Num())
				{
					return false;
				}
				ParserArray[Count] = FString(UE_PTRDIFF_TO_INT32(At - Start), Start);
			}
			Start = At + DelimLength;
			Count += 1;
		}
		if (First)
		{
			ParserArray.Emplace(Start);
		}
		else
		{
			if (Count >= ParserArray.Num())
			{
				return false;
			}
			ParserArray[Count] = FString(Start);
		}

		return Count == ParserArray.Num();
	}

	return false;
}