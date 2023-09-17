#pragma once
#include "UObject/UnrealType.h"
#include <unordered_map>
#include <stdint.h> 

//中间层
class GConfigRowParser
{
public:
	GConfigRowParser() {}
	virtual ~GConfigRowParser() = 0;
public:
	virtual const TArray<FString>& GetParseArray() = 0;
};

class TxtConfigRowParser : public GConfigRowParser
{
protected:
	const FString* m_str;
public:
	TxtConfigRowParser();
	virtual ~TxtConfigRowParser();

	void Reset(const FString& str);
	void Reserve(int Count);
	virtual const TArray<FString>& GetParseArray() override { return ParserArray; }
	int GetId();
private:
	bool ParseIntoArray();
private:
	TArray<FString> ParserArray;
};

/*
 * excel -> protobuf配置文件
 * 生成protobuf读表文件
 * 读取txt配置并序列化binary配置
 * 游戏中读取binary配置
 */
class BinaryConfigRowParser : public GConfigRowParser
{
	virtual ~BinaryConfigRowParser();
};