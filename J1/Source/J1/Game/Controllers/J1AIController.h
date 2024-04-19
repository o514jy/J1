#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "AIController.h"
#include "J1AIController.generated.h"


UCLASS()
class J1_API AJ1AIController : public AAIController
{
	GENERATED_BODY()
public:
	AJ1AIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void ProcessMove(const Protocol::PosInfo& posInfo);

	
};
