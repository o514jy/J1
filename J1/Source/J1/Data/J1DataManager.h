#pragma once

#include "CoreMinimal.h"
#include "J1Data.h"
#include "J1Core.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "J1DataManager.generated.h"


UCLASS()
class J1_API UJ1DataManager : public UGameInstanceSubsystem
{
public:
	GENERATED_BODY()

public:
	void SetInfo();
	
public:
	void ParseAllJsonData();

public:
	FGameplayTag SetTemplateTagByDataId(int32 InTemplateId);
	FGameplayTag SetSkillComponentTagByDataId(int32 InTemplateId);
	FGameplayTag SetSkillTagByDataId(int32 InTemplateId);

	FGameplayTag SetGimmickTagByDataId(int32 InTemplateId);

public:
	UPROPERTY(BlueprintType)
	TObjectPtr<UJ1GameData> GameData;
};
