#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "J1AnimData.generated.h"

class UAnimMontage;

USTRUCT()
struct FJ1AnimEntry
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	int32 OwnerSkillId = 0;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> Montage = nullptr;
};

USTRUCT()
struct FJ1AnimSet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	int32 OwnerDataId = 0;

	UPROPERTY(EditDefaultsOnly)
	TMap<int32, FJ1AnimEntry> SkillIdToAnimEntry;
};

UCLASS()
class J1_API UJ1AnimData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, FJ1AnimSet> OwnerIdToAnimSet;

};
