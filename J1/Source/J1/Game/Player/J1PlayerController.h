#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "J1PlayerController.generated.h"

UCLASS()
class AJ1PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AJ1PlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};