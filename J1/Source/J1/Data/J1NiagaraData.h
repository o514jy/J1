#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "J1NiagaraData.generated.h"

class UNiagaraSystem;

USTRUCT()
struct FJ1NiagaraEntry
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FString Name = FString();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> Niagara = nullptr;
};

UCLASS()
class J1_API UJ1NiagaraData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, FJ1NiagaraEntry> NameToNiagaraEntry;

};
