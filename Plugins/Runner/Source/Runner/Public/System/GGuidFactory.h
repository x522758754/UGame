#pragma once

enum class EGGuidType : uint32
{
	PlayerSuffix = 0x0001,
	NpcSuffix = 0x0002,
	ItemSuffix = 0x0003,
	ClientNpcSuffix = 0x0004,
	ScenePawnSuffix = 0x0005,
	EmitterSuffix = 0x0006,
};


class FGGuidFactory
{
public:
	const static int64 Add = 0x0010;

public:
	int64 GetNextPlayerGuid()
	{
		return FPlatformAtomics::InterlockedAdd(&CurrentPlayerGuid, Add);
	}

	int64 GetNextNpcGuid()
	{
		return FPlatformAtomics::InterlockedAdd(&CurrentNpcGuid, Add);
	}

	int64 GetNextItemGuid()
	{
		return FPlatformAtomics::InterlockedAdd(&CurrentItemGuid, Add);
	}

	int64 GetNextClientNpcGuid()
	{
		return FPlatformAtomics::InterlockedAdd(&CurrentClientNpcGuid, Add);
	}

	int64 GetScenePawnGuid()
	{
		return FPlatformAtomics::InterlockedAdd(&CurrentScenePawnGuid, Add);
	}

	int64 GetNextClientEmitterGuid()
	{
		return FPlatformAtomics::InterlockedAdd(&CurrentEmitterGuid, Add);
	}
public:
	bool IsPlayerGuid(int64 Guid) const {return (Guid & 0x000F) == (uint32)EGGuidType::PlayerSuffix;}
	bool IsClientNpcGuid(int64 Guid) const { return (Guid & 0x000F) == (uint32)EGGuidType::ClientNpcSuffix; }

public:
	static int64 CalculatePlayerGuid(int Index) {return (uint32)EGGuidType::PlayerSuffix + Add * Index;}
private:
	int64 CurrentPlayerGuid = (int64)EGGuidType::PlayerSuffix;
	int64 CurrentNpcGuid =  (int64)EGGuidType::NpcSuffix;
	int64 CurrentItemGuid =  (int64)EGGuidType::ItemSuffix;
	int64 CurrentClientNpcGuid =  (int64)EGGuidType::ClientNpcSuffix;
	int64 CurrentScenePawnGuid =  (int64)EGGuidType::ScenePawnSuffix;
	int64 CurrentEmitterGuid =  (int64)EGGuidType::EmitterSuffix;
};