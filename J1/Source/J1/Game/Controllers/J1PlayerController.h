#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Game/Controllers/J1CreatureController.h"
#include "J1PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class J1_API AJ1PlayerController : public AJ1CreatureController
{
public:
	GENERATED_BODY()
	
public:
	/** network **/
	virtual void ProcessMove(const Protocol::PosInfo& posInfo) override;
};
