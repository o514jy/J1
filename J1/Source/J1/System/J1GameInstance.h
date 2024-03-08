#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "J1GameInstance.generated.h"

class AJ1Player;

UCLASS()
class J1_API UJ1GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Player> OtherPlayerClass;
};
