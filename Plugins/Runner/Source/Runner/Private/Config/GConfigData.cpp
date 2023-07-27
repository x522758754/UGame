// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/GConfigData.h"

#include "System/GAssetManager.h"

TWeakObjectPtr<UGConfigData> UGConfigData::s_Instance = nullptr;
UGConfigData* UGConfigData::Get()
{
	if(!s_Instance.IsValid())
	{
		s_Instance = UGAssetManager::Get()->GetConfigData();
	}

	return s_Instance.Get();
}
