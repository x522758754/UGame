#pragma once
#include "CoreMinimal.h"

#include "ConfigRowParser.h"


class GConfigData
{
	friend class GConfigReader;
public:
	GConfigData();
	virtual ~GConfigData() = 0;
public:
	virtual bool GenerateData(GConfigRowParser* row) = 0;
	virtual int32_t GetParameterCount() {return 0;}
};

class GConfigReader
{
	friend class GConfigManager;
public:
	GConfigReader() {};
	virtual ~GConfigReader() {};
protected:
	virtual GConfigData* AddData(int32 ConfigId) = 0;
	virtual void Reserve(int Num) = 0;
	bool LoadTableText(const FString& cfgPath);

public:
	virtual void Load() = 0;
	virtual void Unload();
};

class UGConfigManager
{
public:
	virtual ~UGConfigManager();

	static UGConfigManager* Get();

	bool Register(const FString& ConfigName, const TSharedPtr<GConfigReader>& Reader);
	void Unregister(const FString& ConfigName);
	GConfigReader* GetReader(const FString& ConfigName);

	
private:
	TMap<FString, TSharedPtr<GConfigReader>> Readers;
	static TSharedPtr<UGConfigManager> Instance;
};
