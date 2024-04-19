#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "J1SkillManager.generated.h"

class UJ1SkillBase;

UCLASS()
class J1_API UJ1SkillManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

public:
	TObjectPtr<UJ1SkillBase> GenerateSkillById(int32 InId);
	
};
