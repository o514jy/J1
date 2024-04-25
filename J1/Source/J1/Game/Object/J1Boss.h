#pragma once

#include "CoreMinimal.h"
#include "Game/Object/J1Monster.h"
#include "J1Boss.generated.h"

UCLASS()
class J1_API AJ1Boss : public AJ1Monster
{
	GENERATED_BODY()
public:

public:
	/** setter & getter **/
	TObjectPtr<UBossData> GetBossData() { return Cast<UBossData>(CreatureData); }
};
