#include "Config/ConfigReader.h"
#include "Config/ConfigRowParser.h"
#include <mutex>
#include <vector>

#include "Misc/FileHelper.h"

std::mutex g_ProviderMutex;

std::vector<std::string> g_stringList;
std::vector<int32_t> g_int32List;
std::vector<uint32_t> g_uint32List;
std::vector<int64_t> g_int64List;
std::vector<uint64_t> g_uint64List;
std::vector<float> g_floatList;
std::vector<bool> g_boolList;

TArray<FString> g_strCfgRows;
TArray<FString> g_strCfgCols;
TArray<FString> g_strCfgArrays;

//////////////////////////////////////////////////////////////////////////

GConfigData::GConfigData()
{
}

GConfigData::~GConfigData()
{
}

//////////////////////////////////////////////////////////////////////////

void GConfigReader::Unload()
{
	
}


bool GConfigReader::LoadTableText(const FString& cfgPath)
{
	FString TextPath = FPaths::ProjectContentDir() + cfgPath;
	TextPath = FPaths::ConvertRelativePathToFull(TextPath);

	g_strCfgRows.Empty();
	TxtConfigRowParser rowPraser;
	if (FFileHelper::LoadFileToStringArray(g_strCfgRows, *TextPath))
	{
		int32 RowNum = g_strCfgRows.Num();
		Reserve(RowNum - 1);
		for(int Row = 1; Row < RowNum; Row++)
		{
			FString& strRow = g_strCfgRows[Row];
			if (strRow.IsEmpty())
			{
				continue;
			}		
			rowPraser.Reset(strRow);
			int Id = rowPraser.GetId();
			if (Id < 0)
			{
				UE_LOG(LogTemp, Error, TEXT("GConfigReader::LoadTableText Praser Failed %s in %s"),
					*strRow, *TextPath);
				continue;
			}
			GConfigData* pData = AddData(Id);
			rowPraser.Reserve(pData->GetParameterCount());
			if (!pData->GenerateData(&rowPraser))
			{
				UE_LOG(LogTemp, Error, TEXT("GConfigReader::LoadTableText GenerateData Failed %s in %s"),
					*strRow, *TextPath);
			}
		}

		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("GConfigReader::LoadTableText %s Load Failed"), *TextPath); 
	return false;
}

/////////////////////////////////////////////////////////////////////////

UGConfigManager::~UGConfigManager()
{
	for(auto &Pair : Readers)
	{
		Pair.Value->Unload();
	}
	Readers.Empty();
}

TSharedPtr<UGConfigManager> UGConfigManager::Instance = nullptr;
UGConfigManager* UGConfigManager::Get()
{
	if(!Instance.IsValid())
	{
		Instance = MakeShareable(new UGConfigManager());
	}
	return Instance.Get();
}

bool UGConfigManager::Register(const FString& ConfigName, const TSharedPtr<GConfigReader>& Reader)
{
	if(Readers.Contains(ConfigName) || !Reader.IsValid())
	{
		return false;
	}
	Reader->Load();
	Readers.Add(ConfigName, Reader);
	return true;
}

void UGConfigManager::Unregister(const FString& ConfigName)
{
	if(GConfigReader* Reader = GetReader(ConfigName))
	{
		Reader->Unload();
	}
	Readers.Remove(ConfigName);
}

GConfigReader* UGConfigManager::GetReader(const FString& ConfigName)
{
	if(!Readers.Contains(ConfigName))
	{
		return nullptr;
	}

	return Readers[ConfigName].Get();
}
