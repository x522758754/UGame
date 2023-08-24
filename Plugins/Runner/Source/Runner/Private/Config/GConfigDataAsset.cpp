// Fill out your copyright notice in the Description page of Project Settings.


#include "Config/GConfigDataAsset.h"

#include "System/GAssetManager.h"

TWeakObjectPtr<UGConfigDataAsset> UGConfigDataAsset::s_Instance = nullptr;
UGConfigDataAsset* UGConfigDataAsset::Get()
{
	if(!s_Instance.IsValid())
	{
		s_Instance = UGAssetManager::Get()->GetConfigData();
	}

	return s_Instance.Get();
}
